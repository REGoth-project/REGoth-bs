.. _object-kinds:

Kinds of Objects in Gothic
==========================

This chapter is a collection of some of the kinds of Objects used in the original
engine, what they are and how they are implemented in REGoth.


Vob
---

The name "Vob" is short for "Virtual Object", which is what Objects placed
into the 3D world are called in the original engine.

Since Gothic uses an Object-Oriented engine, this is usually the class other Objects
are based on.

The most useful properties of a vob include:

- Name
- Transform (Position, Rotation, **no scale**)
- Visual (3D-Model, ParticleSystem, ...)
- (probably more)

As said before, a Vob is one of the most basic building blocks of a 3D-World in
Gothic. For most gameplay relevant object, a subclass of Vob will be placed 
into the world. The raw Vob-class is however used for decorative objects like
3D-Models or particle systems.


NPC (Non-Player-Character)
--------------------------

Even though the name explicitly states, that this is NOT something controlled by the player, in the original Gothic this 
Class is used for real NPCs, monsters, as well as the player. A better name could have been "Character Controller".

The NPC class is rather complex. Some of its tasks are:

- Walking physics and collision detection
- Decisions on which Animation to play
- Inventory management
- Fighting
- Footstep sounds and effects
- Handle dying, being knocked out
- Handle attributes such as Health, Strength, skills
- Some AI related tasks
- Daily routine
- (probably more)


MOB (M...-what?)
----------------

The MOB class is the base for all objects a character could interact with, like doors, beds and chests.

Mobs are constructed using a SkeletalMesh and some Animations, with some bones having a special purpose:
For each spot the character could interact from, there has to be a bone. Upon interaction, the character
will be teleported to the nearest empty interation-spot. Once there, the character and the Mob will start
to play an animation.

Which animation that would be, is defined by the *state* the Mob is in. A chest, for example, has the states:

0. Idle
1. Closed
2. Open

Depending on which state should be activated after an action by the character, a transition animation is played.
For the chest, a transition animation ``0-to-1`` would be the character going down to knee in front of the chest.
Once the character wants to open the chest, a transition animation ``1-to-2`` will be played in which the character
opens the chest.

Once a state has been reached, an animation (typically only 1 frame) is looped over and over again, ie. how the character
knees in front of the chest.