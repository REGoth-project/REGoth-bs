Scene Structure
===============

In the original game, the scene layed out similar to a tree. Each object can have multiple children which will move according to their parent.
Every object is an instance of a class derived from `zCVob` (See :ref:`object-kinds`) and thus have their own behavior. Since REGoth is building upon bs::f, which
allows for using an Entity-Component-System as well as the traditional object-oriented way Gothic is using, we have to decide which way we go.

As far as I understand, using bs::f as object-oriented framework will just create objects with a single component internally. I don't see any reason to
not go for the component based approach.


Logic
-----

Traditionally, the logic of an object in Gothic was given by the type of class it was instanciated of. I would suggest keeping all behavior of an object inside
*Logic-Components*. There will be very few kinds to take into account from the original game:

* Characters
* Interactable objects
* Triggers
* Items on the ground

Of course these could (and should) be split into multiple components. For example, the *Character*-Component could manage the appearance, animations and inventory of a character,
while other components could do AI or handle being controlled by the Player.

Visuals
-------

Since the way rendering works is a little bit different from Gothic, we have to come up with a solution to get Gothics visuals attached to the objects.
In bs::f, there is a *Renderable*-Component, which holds mesh data, materials, etc. For REGoth, I would suggest wrapping that *Renderable*-component into
a *Visual*-Component, which is able to load and digest the original data. It could then save everything needed to display an object as a child-object to keep
other components from directly interacting with what the *Visual*-Component needed to create.

To match the original game, we would need at least the following visuals:

* Static meshes (.3DS)
* Skeletal-Meshes (.ASC)
* Morph-Meshes
* Particle-Effects
 