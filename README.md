# webserv | 42KL

This project involves building a webserver (with functionalities similar to Nginx) that can parse a configuration file and serve clients using HTTP protocol resources such as HTML, CSS, javascript, jpegs, png, pdf files. 
It is fully implemented using C++ object oriented paradigm which uses classes. The project requires use of underlying C system APIs such as Socket(socket programming) and Poll(I/O Multiplexing). The server is built
to simulate real conditions such as:
- high availability (server will not crash once run)
- can handle high throughput (uploading and downloading very large files simultaneously)
- can serve high number of concurrent clients without interruptions.
- use of CGI (Common Gateway Interface) to handle complex client request.

![mandatory gif](https://github.com/mseong123/cub3D/blob/edcc79949299fc4c97ee311ca7bee8140a89f2bf/assets/mandatory.gif)

## Status

Validated 13/12/2023. Grade: 125%. Project duration: 4 weeks.
Team Project by [mseong123](https://github.com/mseong123), [yt1007](https://github.com/yt1007) and [lewislee42](https://github.com/lewislee42)

## Clone

Clone the repository

```
git clone https://github.com/mseong123/webserv.git
```

## Compile and Run

The program is split into a mandatory part and a bonus part. The bonus part adds wall collision, a rotating minimap, a (Sonic!) sprite and the ability to rotate the view by moving the mouse.

To compile the mandatory part, `cd` into the cloned directory and:

```shell
make
```

To compile the bonus part, `cd` into the cloned directory and:

```shell
make bonus
```

To run the program:

For mandatory:
```
./cub3D ./maps/working1.cub
```
For bonus:
```
./cub3D_bonus ./maps/sonic_bonus.cub
```
## Controls

Controls for movement and rotation are:

- `W`: move forward
- `S`: move backward
- `A`: strafe left
- `D`: strafe right
- `left arrow`: rotate left
- `right arrow`: rotate right
- `mouse`: rotate by moving the mouse (bonus only)
- `ESC`: exit the program

## Useful Resources

Useful resources for this project are:

- Raycasting: [Lode's Computer Graphics Tutorial - Raycasting](https://lodev.org/cgtutor/raycasting.html)
- Raycasting: [Permadi's Raycasting tutorial](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)
- MinilibX tutorial : [harm-smits.github.io](https://harm-smits.github.io/42docs/libs/minilibx.html)
- [Subject PDF](https://github.com/mseong123/cub3D/blob/main/en.subject.pdf)


---
Team Project by [mseong123](https://github.com/mseong123) and [lewislee42](https://github.com/lewislee42)
