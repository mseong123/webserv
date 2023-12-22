# webserv | 42KL

This project involves building a webserver (with functionalities similar to Nginx) that can parse a configuration file and serve clients using HTTP protocol resources such as HTML, CSS, javascript, jpegs, png, pdf files. 
It is fully implemented using C++ object oriented paradigm which uses classes. The project requires use of underlying C system APIs such as Socket(socket programming) and Poll(I/O Multiplexing). The server is built
to simulate real conditions such as:
- high availability (server will not crash once run)
- can handle high throughput (uploading and downloading very large files simultaneously)
- can serve high number of concurrent clients without interruptions.
- use of CGI (Common Gateway Interface) written in C++ and Python to handle complex client request.
- use of cookies and session.

## Definitions
### What is a I/O Multiplexing ?
I/O multiplexing is a programming technique that allows a single process to efficiently manage multiple I/O operations concurrently without the need for multiple threads or processes. It is commonly used in networking applications where a program needs to handle multiple connections simultaneously. Instead of blocking on each I/O operation, I/O multiplexing uses mechanisms like select(), poll(), or epoll() on Unix-like systems to monitor multiple file descriptors for activity. When data is ready for reading or writing on any of the file descriptors, the program can then perform the corresponding I/O operation. This approach enhances the program's responsiveness and resource efficiency compared to traditional blocking I/O, making it well-suited for high-performance network applications.

### What is CGI ?
The Common Gateway Interface (CGI) is a standard protocol that enables communication between web servers and external programs, allowing dynamic content generation on web pages. When a user requests a CGI-enabled web page, the web server invokes the specified CGI program, passing relevant data such as user inputs. The CGI program processes the data, generates dynamic content (e.g., HTML), and returns it to the web server, which, in turn, sends the result to the user's browser. CGI facilitates the integration of server-side scripts or programs with web servers, enabling the creation of interactive and dynamic web applications by executing programs in response to user requests.

### What is HTTP protocol ?
The Hypertext Transfer Protocol (HTTP) is the foundation of data communication on the World Wide Web. It is a standard protocol used for transferring hypertext, which includes text, images, links, and other multimedia content, between web browsers and servers. HTTP operates as a request-response protocol, where a client (typically a web browser) sends requests for resources, and a server responds with the requested data. The communication is text-based and stateless, meaning each request from the client is independent of previous requests. HTTP defines methods like GET and POST for different types of requests, and it uses Uniform Resource Identifiers (URIs) to specify resources. HTTPS, a secure variant of HTTP, incorporates encryption for secure data transmission.

## Status

Validated 13/12/2023. Grade: 125%. Project duration: 4 weeks.
Team Project by [mseong123](https://github.com/mseong123), [yt1007](https://github.com/yt1007) and [lewislee42](https://github.com/lewislee42)

## Clone

Clone the repository

```
git clone https://github.com/mseong123/webserv.git
cd webserv
```

## Compile and Run

```shell
make
```

A default config file (default_config) has already been written and will be parsed automatically by the program to run. To run the program:

```shell
./webserv
```

## Key Features

1) To serve a (already prepared) static website, in browser(Chrome/Firefox/Safari) type:
```shell
http://127.0.0.1:8010/webpage/
```
2)




---
Team Project by [mseong123](https://github.com/mseong123) and [lewislee42](https://github.com/lewislee42)
