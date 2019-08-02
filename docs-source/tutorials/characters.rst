Characters
==========

When talking about Characters, called *NPCs* in the original engine, the people and monsters inside
the world are meant.  A character can be the Hero, Diego or some random other dude minding his own
business.  But it can also be a Wolf, an Orc or a Meatbug.  In REGoth, a each Character has its own
Scene-Object, with multiple Components working together in order to process the AI, render the
visuals and a number of other things.

.. note::

   The original engine called all of those *NPCs*, as in "Non Player Characters". However, that name
   is misleading as the Character controlled by the player was an *NPC*, too.  Therefore, we just
   call everything *Character* in REGoth.


Inserting a Character
---------------------

If you want to insert a character into the world via native code, you should take a look at the
``GameWorld``-class, more specifically at ``GameWorld::insertCharacter()``. There exist multiple
overloads of that method, where each of it takes the Name of the Script-*Instance* to create and a
location where to spawn it.  For example, ``insertCharacter("PC_HERO", "OC1")`` will insert a
character of instance ``PC_HERO`` at the Waypoint ``OC1``, which is near the Old-Camp.  The name of
the Script-Instance comes from within the Daedalus-Scripts.


Inserting the Hero
------------------

To insert the hero, you need to:

1. Insert a Character with the ``PC_HERO`` instance,
2. register it as *Hero*,
3. optionally attach a ``CharacterKeyboardInput``-Component to control it.

Once you have loaded a world, this is done by the following lines of code:

.. code-block:: c

   HCharacter hero = world->insertCharacter("PC_HERO", "STARTPOINT");
   hero->useAsHero();
   hero->SO()->addComponent<CharacterKeyboardInput>();

The parameter ``"STARTPOINT"`` has to be set to a valid Waypoint in this case.  You can also use an
other overload to use a different method to describe where the Hero should be added.

.. note::

   After the hero is created and registered, you may call ``GameWorld::runInitScripts()`` to also
   insert the other characters and items into the world.


Shrinking and Un-Shrinking
--------------------------

The original engine can save processing time by not updating Characters which are far away from the
player.  If a character leaves a certain radius, it's visuals are unloaded after some time, physics
is no longer processed and AI is disabled.  Disabling a Character is called *Shrinking* by the
original engine.  Once a character comes into the players visibility radius again, it is
*Un-shrinked* and being fully processed again.

While a Character is *Shrinked*, it will still exist within the world and can be targeted by script
code as well as native code.


Distances
~~~~~~~~~

As mentioned before, once a Character reaches a certain distance to the player, it will be
*Shrinked*.  If the distance to the player falls below that threshold again, it will be
*Un-Shrinked*.

However, it's not actually that simple.  The original engine, actually has *two* distances:

- one for Characters going *out of range* (*Shrink*),
- and one for Characters coming *into range* (*Un-Shrink*),

while the distance for *Shrinking* is larger than the one for *Un-Shrinking*. This is to work around
that when using a single distance, Characters being right on the edge of it may pop into and out of
range every other frame.

By default, Gothic *Shrinks* Characters being further away than 45 meters and *Un-Shrinks* them
again if they come closer than 40 meters.


Positioning of Shrinked Characters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Since *Shrinked* Characters have their AI disabled, they cannot move along their daily routine.
Once a character goes out of range, it would stay exactly where it was until the player comes close
to it again.  To work around that, the daily routines of the Characters will still be evaluated
while the Character is *Shrinked*. The position of a Character the player cannot see is then just
assumed to be whatever the current routine says.

.. note::

   This is why in Gothic I, you can run into the Old-Camp faster than Diego walks, but you will
   still find him at his hut: He went out of reach since he walks rather slowly, got put where
   his daily routine expects him to be while being *Shrinked*, which is at his own hut and therefore
   arrived faster at the Old-Camp than you can run.

   The only way to arrive faster than Diego is to always keep him in range, so he doesn't get
   *Shrinked* and then move a little bit quicker on the last few steps.


Components making up a Character
--------------------------------

The following Components are important for a Character:

 - ``VisualCharacter`` -
   Loads the Skeletal-Mesh and handles Animation-Playback.

 - ``bs::CCharacterController`` -
   Physics-Processing.

 - ``CharacterAI`` -
   Implements the Characters State-Machine, i.e. which actions it can do at what time and asks the
   visual to play matching animations.

 - ``Character`` -
   Handles the Characters gameplay relevant actions, and serves as the main interface into what a
   Character can do.  This is also the place where most `NPC_*`-externals are implemented.

 To create a Scene-Object with the correct components, please see ``GameWorld::insertCharacter()``.
