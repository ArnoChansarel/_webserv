/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestLine.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achansar <achansar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 20:28:18 by nesdebie          #+#    #+#             */
/*   Updated: 2024/04/17 16:01:52 by achansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestLine.hpp"

RequestLine::RequestLine() {}

RequestLine::RequestLine(int const &method, std::string const &path, std::string const &http_version, std::string const &methodName) : _method(method), _path(path), _http_version(http_version), _query("") {
    if (_method == UNVALID)
        _not_valid = methodName;
    else
        _not_valid = "";
    vec_str arr = _vectorSplit(path, '?');
    if (arr.size() == 2) {
        _path = arr[0];
        _query = arr[1];
    } else {
        _path = arr[0];
    }
}

RequestLine::RequestLine(const RequestLine & copy) {
    *this = copy;
}

RequestLine::~RequestLine() {
}

/* ----- FUNCTIONS ----- */
vec_str RequestLine::_vectorSplit(std::string str, char sep) {
    vec_str arr;
    char*   cstr = const_cast<char*>(str.c_str());
    char*   token = std::strtok(cstr, &sep);

    while (token != 0) {
        arr.push_back(token);
        token = std::strtok(0, &sep);
    }
    return arr;
}

/* ----- GETTERS ----- */

int RequestLine::getMethod() const {
    return _method;
}

std::string RequestLine::getPath() const {
    return _path;
}

std::string RequestLine::getHTTPVersion() const {
    return _http_version;
}

std::string RequestLine::getNotValid() const {
    return _not_valid;
}

std::string RequestLine::getQuery() const {
    return _query;
}

/* ----- OPERATORS ----- */

RequestLine& RequestLine::operator=(RequestLine const &op) {
    if (this != &op) {
        _method = op._method;
        _path = op._path;
        _http_version = op._http_version;
        _not_valid = op._not_valid;
    }
    return *this;
}

std::ostream & operator<<(std::ostream &o, RequestLine const &obj) {
    std::string httpMethods[4] = {"DELETE", "GET", "POST", obj.getNotValid()};
	o << httpMethods[obj.getMethod()]<< ": " << obj.getPath() << " " << obj.getHTTPVersion() << std::endl;
    return o;
}