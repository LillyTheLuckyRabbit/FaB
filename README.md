# FaB (Fun and Balanced)

## Compiling
To compile, use the enclosed Makefile in the 'fab' folder. On Arch Linux / Manjaro, the following packages are required:

- sdl2
- sdl2_mixer
- sdl2_image

On Windows, you can compile using MinGW. Check this link for more info: https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/mingw/index.php

## Starting the Game
The game requires two XInput controllers to be attached to the computer. If only one (or zero) are attached, the game will not start. **This is not a bug.** Up to four controllers are supported at a time.

PRO TIP: On Linux, you can use the userland "xboxdrv" driver to have non-XInput controllers mimic XInput controllers. Optionally, you can also use Steam, but your mileage may vary depending on the controller. From our experience, "xboxdrv" does better with lesser known controllers while Steam has good support for mainstream controllers (Dualshock 4, Switch Pro, etc).

## Controls
- **Left Stick:** Move
- **Right Stick:** Aim
- **Left Trigger:** Dig
- **Right Trigger:** Shoot
- **Left Bumper:** Dash
- **Right Bumper:** Jump

## How to Play
FaB is a 2D "player vs. player" deathmatch game. Terrain is randomly generated and the players are dropped in. Each player is armed with four different weapons. After a player "frags" another player using a weapon of their choice, they are awarded a point. The terrain is fully destructible, so it can be changed by weapons and by players digging. Projectiles can be reflected by using the "dash." Dashing not only provides enhanced mobility, but also creates a shield to reflect projectiles.

There are four weapons in the game:
1. **Six Shooter**
  - The Six Shooter is the fastest weapon and has the best range.
2. **Flamethrower**
  - The Flamethrower's range is abysmal, but up close, it can wipe out a player with full health in a split second.
3. **Shotgun**
  - The Shotgun has slightly better range than the Flamethrower and the cost of spread and worse damage.
4. **Grenade Launcher**
  - The Grenade Launcher can destroy large chunks of terrain and players with its high blast radius. The downside is, the grenade has an arcing path by virtue of being heavy.
