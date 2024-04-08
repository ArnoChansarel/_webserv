/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:58:55 by achansar          #+#    #+#             */
/*   Updated: 2024/04/08 16:33:14 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// ============================================================================== CONSTRUCTORS

Response::Response(Server* server, int statusCode, Request* request, const int socket) :
        _clientSocket(socket),
        _method(request->getMethod()),
        _statusCode(statusCode),
        _path("/"),
        _server(server),
        _request(request) {

    return;
}

Response::~Response() {
    return;
}

// ============================================================================== MEMBER FUNCTIONS

// ==================================================================== FILE TRANSFER

int Response::sendFile() {

	std::ifstream	infile(_path, std::ios::binary | std::ios::in);
	if (!infile) {
		std::cout << "Le fichier ne s'ouvre pas." << std::endl;
		return 500;
	} else {
		infile.seekg(0, std::ios::end);
		std::streampos fileSize = infile.tellg();
		infile.seekg(0, std::ios::beg);

        std::string fileName = extractFileName();

		std::stringstream responseHeaders;
			responseHeaders << "HTTP/1.1 200 OK\r\n";
			responseHeaders << "Content-Type: " << getMimeType() << "\r\n";
			responseHeaders << "Content-Disposition: attachment; filename=\"" << fileName << "\"\r\n";
            responseHeaders << "Content-Length: " << fileSize << "\r\n\r\n";
			
		write(_clientSocket, responseHeaders.str().c_str(), responseHeaders.str().length());
		const std::streamsize bufferSize = 8192;
		char buffer[bufferSize];

        while (!infile.eof()) 
            {
                infile.read(buffer, sizeof(buffer));
                ssize_t result = write(_clientSocket, buffer, infile.gcount());
                if (result == -1) 
                {
                    std::cerr << "Error writing to socket." << std::endl;
                    break;
                }
            }

        infile.close();
	}
	return 200;
}

/*

TO DO (Arno)
	work on 300 reidrections
    build deleteFiles
	do autoindexes
*/

int Response::receiveFile() {

    std::cout << "IN RECEIVE FILE\n";
    
    std::stringstream rawRequest(_request->getRaw());
    std::string line;
    std::string fileName;
    size_t Pos = 0;

    while (std::getline(rawRequest, line) && !line.empty()) {
        if (line.find("Content-Disposition") != std::string::npos) {
            Pos = line.find_last_of("=");
            if (Pos != std::string::npos) {
                size_t endPos = line.find_last_of('"');
                fileName = line.substr(Pos + 2, endPos - Pos - 2);
            }
        }
    }

    std::string fileBody = extractFileBody(_request->getRaw());
    // if (fileBody.empty()) {
    //     return 400;
    // }
    std::string destination = "." + _request->getPath() + "/" + fileName;
    // std::cout << "dest : " << destination << std::endl;
    // std::ofstream outputFile(destination);
    // if (!outputFile) {
    //     std::cerr << "Error creating file" << std::endl;
    //     return 500;
    // }
    // outputFile << fileBody;
    // outputFile.close();
                
    std::ofstream targetFile(destination, std::ios::binary);
    if (!targetFile) {
        std::cerr << "Error creating the file.\n";
    }
    targetFile.write(fileBody.c_str(), fileBody.size());
    targetFile.close();
	return 200;
}

int Response::deleteFile() {
    
    // if (std::remove(_path) != 0) {
    //     std::cerr << "Error deleting file" << std::endl;
    //     return 500;
    // }
    return 204;
}

int Response::fileTransfer() {//           FAUT-IL UTILISER SELECT/POLL ?

    std::cout << "In file transfer, method is " << _method << std::endl;
    switch (_method) {
        case GET:       return sendFile();
        case POST:      return receiveFile();
        case DELETE:    return deleteFile();
        default:        return 200;//                METHOD UNKNOWN, what statusocde ?
    }
}

// ==================================================================== GET METHOD

std::string Response::getReason(int sc) {

    std::map<int, std::string> reasons;
    reasons.insert(std::make_pair(200, "OK"));
    reasons.insert(std::make_pair(201, "Created"));
    reasons.insert(std::make_pair(204, "No Content"));
    reasons.insert(std::make_pair(400, "Bad Request"));
    reasons.insert(std::make_pair(403, "Forbidden"));
    reasons.insert(std::make_pair(404, "Not Found"));
    reasons.insert(std::make_pair(500, "Internal Server Error"));

    std::map<int, std::string>::iterator it = reasons.find(sc);
    if (it == reasons.end()) {
        std::cout << "Status code not implemented !" << std::endl;
    }
    return it->second;
}

std::string Response::getHeaders(const int s) {
    std::string h;

    if (_method == GET) {
        h += "Content-Type: " + getMimeType() + "\r\n";
    }
    h += "Content-Length: " + std::to_string(s) + "\r\n";// virer tostirng
    return h;
}

bool Response::isDirectory(std::string path) {
    struct stat fileStat;
    if (path[_path.size() - 1] != '/')
                path += "/";
    if (stat(path.c_str(), &fileStat) == 0) {
        return S_ISDIR(fileStat.st_mode);
    }
    return false;
}

int Response::generateAutoindex() {

    std::stringstream response;
    
    if (_path[_path.size() - 1] != '/')
        _path += "/";

    DIR* dir = opendir(_path.c_str());
    if (!dir) {
        std::cerr << "Error opening directory.\n"; 
        return 500;
    }

    response << "<html><head><title>Autoindex</title></head><body>";
    response << "<h1>Autoindex</h1>";
    response << "<ul>";

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            response << "<li>" << name << "</li>";
        }
    }

    closedir(dir);
    response << "</ul></body></html>";
    _body = response.str();
    return 200;
}

void Response::getBody(bool autoindex) {

	std::ifstream			myfile;
    std::string             line;

    if (_method == GET) {
        if (isDirectory(_path)) {
            std::cout << "IS A DIRECTORY : " << _path << std::endl;
            if (autoindex) {
                std::cout << "AUTOINDEX ON\n\n";
                _statusCode = generateAutoindex();
            } else {
                std::cout << "AUTOINDEX OFF\n\n";
                _statusCode = 403;
            }
        } else {
            std::cout << "IS A FILE : " << _path << std::endl;
            myfile.open(_path);
            if (myfile.fail()) {
                _statusCode = 500;
            }

            while (std::getline(myfile, line)) {
                _body += line;
            }

            myfile.close();
        }
    } else if (_method == POST) {
        _body = "";
        // might be infos :
        // resource creation & confirmation message
    } else if (_method == DELETE) {
        _body = "";
    }
}

// ==================================================================== SWITCH


void      Response::buildResponse(Route *route) {

    bool autodindex = false;
    if (route) {
        autodindex = route->getAutoindex();
        
    }
    _extension = extractExtension(_request->getPath());
    getFullPath(route, _request->getPath());
    
    std::stringstream   ss;

    std::cout   << "\n\nREQUEST IN BUILD:\n------------------------------------------------------------------------------------------------------\n"
                << _request->getRaw()
                << "\n------------------------------------------------------------------------------------------------------\n\n" << std::endl;
    
    if ((!_extension.empty() && _extension.compare(".html")) || _method != GET) {
            _statusCode = fileTransfer();
    }
    
    if (_statusCode == 200) {
        getBody(autodindex);
        _headers = getHeaders(_body.length()) + "\n";
        ss << _statusCode;
        _statusLine = "HTTP/1.0 " + ss.str() + " " + getReason(_statusCode) + "\n";
    } else {
        buildErrorResponse();
    }
    _responseLine = _statusLine + _headers + _body;
    std::cout << "\nRESPONSE :: \n" << _responseLine << std::endl;
    return;
}

// ============================================================================== GETTER & SETTER

std::string     Response::getResponse() {
    return _responseLine;
}

std::string     Response::getPath() {
    return _path;
}

int             Response::getStatusCode() {
    return _statusCode;
}

void            Response::setPath(std::string& str) {
    _path = str;
    return;
}

void            Response::setErrorPath(std::string& str) {
    _errorPath = str;
    return;
}

// ============================================================================== UTILS

std::string Response::extractExtension(std::string uri) {
    
    std::cout << "In extract extension, path : " << uri << std::endl;
    size_t extPos = uri.find_last_of(".");
    if (extPos != std::string::npos) {
        std::string extension = uri.substr(extPos, std::string::npos);
        return extension;
        std::cout << "EXTENSION IS : " << extension << std::endl;    
    } else {
        std::cerr << "Couldn't extract extension.\n";
        return "";
    }
}

std::string Response::extractFileName() {
    
    size_t extPos = _path.find_last_of("/");
    if (extPos != std::string::npos) {
        std::string fileName = _path.substr(extPos + 1, std::string::npos);
        return fileName;    
    } else {
        std::cerr << "Couldn't extract file name.\n";
        return NULL;
    }
}

std::string Response::extractFileBody(std::string request) {
    
    size_t boundaryPos = request.find("Content-Type");
    if (boundaryPos == std::string::npos) {
        return "";
    }
    size_t Pos = request.find("\n", boundaryPos) + 3;
    if (Pos == std::string::npos) {
        return "";
    }
    size_t endPos = request.find("\n------WebKitFormBoundary", Pos) - 1;
    if (endPos == std::string::npos) {
        return "";
    }
    std::string fileBody = request.substr(Pos, endPos - Pos);
    return fileBody;
}

std::string     Response::getMimeType() {

    if (!_extension.empty()) {
        return _server->getMimeType(_extension);
    }
    return _server->getMimeType("default");
}

void	Response::getFullPath(Route *route, std::string uri) {

	if (route) {
        _path = route->getRoot();
        if (!route->getAutoindex()) {
            _path += "/" + route->getIndex().front();
        }
	} else {
        if (isDirectory("." + uri)) {
            _path = "." + uri;
        } else {
            _path = (_extension != ".html") ? "./download" + uri : "./docs" + uri;
            std::ifstream myfile(_path.c_str());
            if (myfile.fail()) {
                _statusCode = 404;
                _path = "/";
            }
        }
	}
    // std::cout << "END OF GETFULLPATH , PATH IS [" << _path << "]\n";
	return;
}
