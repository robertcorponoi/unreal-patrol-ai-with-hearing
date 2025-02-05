<p align="center">
  <img width="876" height="387" src="https://github.com/robertcorponoi/graphics/blob/master/unreal-creating-a-patrol-ai-with-hearing-sense/testing.png?raw=true">
</p>

<h1 align="center">Unreal C++ Patrol AI With Hearing Sense</h1>

<p align="center">Demo project for my tutorial about creating a patrol AI with hearing sense using C++.<p>

This is the demo project for the tutorial at [Unreal C++ Creating A Patrol AI With Hearing Sense](https://robertcorponoi.com/unreal-c-creating-a-patrol-ai-with-hearing-sense/) which explains how to set up a patrolling AI that listens for the player making sound. This is a good setup for any game that might involve enemies that track the player via sound instead of line of sight.

This demo was created with Unreal 4.25.3

## Instructions

All you have to do is clone or download the repo and open it up in Unreal. By default if you hit play the enemy will detect the player's footsteps so if you wanted to do some testing with single sounds you can go into the `PlayerCharacter.cpp` script and remove the line that makes noise in the `PlayerCharacter::MoveForwardBackward` method and instead place it in the crouch method so that the player makes a single noise when crouching and the AI will investigate it.

Also make sure you tinker with the values for the `AIHearingRadius` and `AIHearingMaxAge` to see the different results it produces.

## **License**

MIT
