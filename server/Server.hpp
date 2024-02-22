/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mprofett <mprofett@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 11:15:25 by mprofett          #+#    #+#             */
/*   Updated: 2024/02/22 15:55:54 by mprofett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <list>
# include <map>
# include <vector>
# include <cstdlib>
# include <unistd.h>
# include "Route.hpp"

# define LOCALHOST "127.0.0.1"

class Server
{
	public:
		class	invalidPortRange : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidRootDirectory : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidErrorPageCode : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidErrorPagePath : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class	invalidHostFormat : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		Server();
		~Server();

		std::list<Route>			getRoute(void) const;
		std::list<std::string>		getServerNames(void) const;
		std::map<int, std::string>	getErrorPages(void) const;
		std::list<std::string>		getIndex(void) const;
		std::string					getIpAdress(void) const;
		int							getPort(void) const;
		std::string					getRoot(void) const;

		void						addRoute(Route route);
		void						addServerName(std::string name);
		void						addErrorPage(int code, std::string path);
		void						addIndex(std::string filename);
		void						setIpAdress(std::string ip);
		void						setPort(int port);
		void						setRoot(std::string path);

		std::string					convertIpAddress(std::vector<std::string> address);
		void						printDatas(void) const;

	private:
		std::list<Route>			_routes;
		std::list<std::string>		_server_names;
		std::list<std::string>		_index;
		std::map<int, std::string>	_error_pages;
		std::string					_ipAdress;
		std::string					_root;
		int							_port;
};

#endif
