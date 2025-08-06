# Project Overview

**This is a repo made from following Stephen Ulibarri (the goat)'s Unreal Engine C++ course.**

## Architectural Differences
Where my project differs from what you would get from just following along, is mostly its focus on modularity, decoupling and low level performance.
My game code is divided into modules for easy integrating into my upcoming projects. It is also great for keeping my code organized, and least of all, my compile times low. My philosophy for game programming has therefore recently become  
_**Keep it modular, or don't keep it at all**_

## Course Notes
Section 14:
- When applying transient fields (physics fields) right after one another, you might not get the expected functionality when you need one thing to happen before the other. This is before the game calculated the state of the game each tick, and so, in the game's eyes, those actions happened simultaneously. The solution here is to use a delay, timer, or my favorite, timer for the next tick. That way, you ensure the actor has been modified by the transient field, and can, in this case, apply a linear force to scatter the broken parts of the vase.
