/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nesdebie <nesdebie@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/04/15 15:26:52 by nesdebie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "../TcpListener.hpp"
# include "../../request_manager/includes/Cgi.hpp"

void	TcpListener::runTcpListener()
{
	bool	running = true;
	fd_set	ready_to_read_fds;
	fd_set	ready_to_write_fds;
	Server	*server;

	while (running)
	{
		ready_to_read_fds = this->_read_master_fd;
		ready_to_write_fds = this->_write_master_fd;
		if (select(FD_SETSIZE, &ready_to_read_fds, &ready_to_write_fds, NULL, NULL) < 0)
			throw socketSelectFailure();
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_to_read_fds))
			{
				server = this->getServerBySocket(i);
				if (server)
					handleNewConnection(server);
				else
				{
					readRequest(i);
					if (FD_ISSET(i, &_write_master_fd))
						handleRequest(i);
				}
			}
			else if (FD_ISSET(i, &ready_to_write_fds))
				writeResponse(i);
		}
	}
}

void	TcpListener::handleNewConnection(Server *server)
{
	std::cout << "Handling new connection" << std::endl;
	int			client_socket;
	sockaddr	client_addr;
	socklen_t	addr_size = sizeof(sockaddr);

	client_socket = accept(server->getSocket(), &client_addr, &addr_size);
	if (client_socket < 0)
		throw socketAcceptingNewConnectionFailure();
	FD_SET(client_socket, &this->_read_master_fd);
}

void	TcpListener::handleRequest(int client_socket)
{
	if (_pending_request.getMethod() == POST)
	{
		std::cout << "Method is POST" << std::endl;
		std::cout << "Body is: " << _pending_request.getBody().size() << "Content lenght is: " << _pending_request.getContentLength() <<std::endl;
	}
	int status_code = 200;
	Route *route = NULL;
	Server *server = getServerByHost(getPortBySocket(&client_socket), _pending_request.getHeader("Host"));

	std::string checkRoute = _pending_request.getPath();
	if (!checkRoute.empty() && checkRoute[checkRoute.size() - 1] != '/')
		checkRoute += "/";

	std::list<Route *> r = server->getRoute();
	for (std::list<Route *>::iterator it = r.begin(); it != r.end(); it++) {
		if (!(*it)->getPath().compare(checkRoute) || !((*it)->getPath() + "/").compare(checkRoute)) { // la deuxieme condition necessaire car ne rentre pas dans cgi sinon
			route = *it;
			break;
		}
	}

	if (route) {
		if ((!route->getExtension().empty()) || _pending_request.getMethod() == POST || (_pending_request.getMethod() == GET && _pending_request.getPath().compare("/"))) {
				std::cout << "[CGI] START" << std::endl;
				try {
					Cgi cgi(_pending_request, *route);
					std::string tmp = cgi.executeCgi();
					std::cout << "[CGI] END ===> [SATUS CODE = " << status_code << "]" << std::endl;
					status_code = cgi.getExitCode();
					Response response_cgi(server, cgi.getExitCode(), &_pending_request, client_socket);
					std::string path = _pending_request.getPath();
					response_cgi.setPath(path);
					response_cgi.buildResponse(route);
					FD_SET(client_socket, &this->_write_master_fd);
					this->registerResponse(client_socket, response_cgi);
					std::cout << "------------CGI-------------" << std::endl;
					return ;
				}
				catch(std::exception &e) {
					std::cout << e.what() << std::endl;
				}	
		}
		
	}

	Response response(server, status_code, &_pending_request, client_socket);//                create response here
	response.buildResponse(route);
	FD_SET(client_socket, &this->_write_master_fd);
	this->registerResponse(client_socket, response);
}

void	TcpListener::registerResponse(int socket, Response &response)
{
	std::map<int, Response>::iterator	it;

	it = this->_responses.find(socket);
	if (it == this->_responses.end())
		this->_responses.insert(std::pair<int, Response>(socket, response));
	else
		(*it).second = response;
}

void	TcpListener::writeResponse(int client_socket)
{
	std::cout << "Sending response" << std::endl;
	std::string	response = this->_responses.find(client_socket)->second.getResponse();

	send(client_socket, response.c_str(), response.size(), 0);
	this->_responses.erase(client_socket);
	FD_CLR(client_socket, &this->_write_master_fd);
}
