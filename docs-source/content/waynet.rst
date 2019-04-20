Waynet
======

This chapter will discuss what the waynet is, what it is used for and how to access it in REGoth.

Most of waynet-related operations can be found inside the components ``Waynet``
and ``Waypoint`` defined in

 - ``src/components/Waynet.hpp`` and
 - ``src/components/Waypoint.hpp``, respectively.

What is the Waynet?
-------------------

The Waynet's main use case is to allow AI controlled characters to navigate
through the world. It tells the AI which paths can be taken to reach a certain
destination. Think of it like a *street-map* for the AI. See :numref:`fig-waynet` for
an example.

.. _fig-waynet:
.. figure:: images/waynet.png
   :alt: Waynet of the Old-Camp in Gothic 2

   Waynet of the Old-Camp in Gothic 2 visualized. The places at the
   intersections are called *Waypoints*. A line from one *Waypoint* to another
   means that there is a path between them the AI can take.

This *street-map* is built up from *Waypoints* and *Paths*. Waypoints are normal
Scene-Objects with the ``Waypoint``-component attached. This component stores
references to all other Waypoints that can be reached directly from its
location. If a Waypoint is in reach of another Waypoint, that means there is a
*Path* between the two.

The scripts actually only tell the target destination a character should go to,
so some sort of algorithm to find the shortest path to it is needed.

The Waynet-Components
---------------------

Once a world has been loaded (See :ref:`world`), the worlds Waynet can be queried by
``REGoth::gWorld().waynet()``. This will return a handle of a ``Waynet``-Component.

This ``Waynet``-component offers a list of all known Waypoints and functions to search for
specific ones. Since every Waypoint is a normal Scene-Object, you can also simply do a
recursive search for the name you are looking for from the scenes root object.

.. note::

   If you are querying a Waypoint by name and you are not *completely* sure that the object you are
   looking for is indeed a Waypoint, it's better to search the full scene.

If you have a ``Waypoint``-component at hand, you can query the paths reachable from that Waypoint
from it.

``TODO: Pathfinding should also be implemented in the waynet, or at least accessible from it.``


How Pathfinding works
---------------------

Pathfinding describes the process of finding the shortest path between two locations. Those two
locations do not need to be directly *on* the waynet, but a Waypoint should be in reach for
the start and end locations.

A naive solution would be to simply use a shortest-path algorithm like *Dijkstras-Algorithm* to find the
sequence of Waypoints that would get the AI to its destination.

It turns out that this is not the full story. In Gothic, AI controlled characters will often not use the
shortest possible path, but go an other route. How that is determined is unknown.

For example: When using the shortest path possible, the character showing the way from the old
camp to the swamp will go though the forest, while he goes around the forest in
the original.

.. note::

   Since the Waynet is supposed to be static, it might be beneficial to precompute the distances
   between Waypoints and store them in an acceleration structure for faster pathfinding.

``TODO: Need more information.``
