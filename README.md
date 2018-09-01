# isoLand

## The project
Conceptualize a 2D isometric video game, open and tiled map. Cross-platform (desktop and mobile).

## Development
- Game framework : Cocos2d-x - [official website](http://cocos2d-x.org/) [github](https://github.com/cocos2d/cocos2d-x/)
- Language : C++

## Features
- Movable player
- Environment collisions (rocks, trees...)
- Path finding (algorithm A*)
- Open world
- Tiled map
- Sprite animation

## Graphics tools
- [Tiled](http://www.mapeditor.org/)
- [TexturePacker](https://www.codeandweb.com/texturepacker)

## Installation
- download project

```
$ git clone https://gitlab.com/ProjetsHUB/Dommerc.git
```

or https://gitlab.com/ProjetsHUB/Dommerc

- install Cocos2d-x
  
Follow the steps: http://cocos2d-x.org/docs/cocos2d-x/en/installation/

## Build&Run Linux
- if you never run cocos2d-x before

```
$ cd NEW_PROJECTS_DIR/cocos2d/build
$ ./install-deps-linux.sh
```

- run

```
$ cd NEW_PROJECTS_DIR
$ cocos run -p linux
```

- for other dev environments see https://github.com/cocos2d/cocos2d-x

## CLion IDE (for Linux desktop development)
If you encounter this error while run the app  
`error while loading shared libraries: libfmod.so.6: cannot open shared object file: No such file or directory`  
Be sure to install libfmod.so.6 and set this environment variable to your run/debug config:  
Name `LD_LIBRARY_PATH` Value `$LD_LIBRARY_PATH:/usr/local/lib` (if the lib is located under /usr/local/lib)

## How to contribute ?
First of all, if you want extend the project, add a feature, play with it, whatever, please **make a new branch** !
For push your changes to the master, **make a pull request**.

To code the right place is in the **Classes** directory at the root of the repository.
This code is shared between all the different deployments (Android, IOS, Win-Linux-Mac desktop...).
This is the relevant code of the project.

Assets (like textures, sounds, fonts...) are located in **Resources** directory at the root of the repo.

*Enjoy !*