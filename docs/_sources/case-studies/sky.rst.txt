.. _sky:

Sky
===

The sky consists of several layers, and uses different primitives as model depending on the game
and graphic settings.
The original Gothic game only used a plane to render the sky texture, while Gothic II added the
possibility to render the sky as a dome, given enough processing power.

The different layers of the sky are (1), a plain color layer as background (:numref:`fig-sky1`),
(2) a color layer additively blended in (:numref:`fig-sky2`), and (3) the actual alpha-blended sky
texture (:numref:`fig-sky3`).

.. _fig-sky1:
.. figure:: images/sky_1_raw_bg_color.png
   :alt: Sky raw background color.

   Background sky color (1) in Gothic II.

.. _fig-sky2:
.. figure:: images/sky_2_color_layer.png
   :alt: Sky raw background color and color layer.

   Background sky color (1) together with an additively blended color layer (2) rendered as cylinder
   in Gothic II.

.. _fig-sky3:
.. figure:: images/sky_3_clouds_layer.png
   :alt: Sky raw background color, color layer, and texture layer.

   Background sky color (1) together with an additively blended color layer (2) rendered as
   cylinder, as well as the alpha-blended sky texture (3) layer in Gothic II.


The additively blended color layer (2) is necessary, because the sky looks dull otherwise.  See, for
example, :numref:`fig-sky-no-color-layer`, where this layer was deactivated.  In
:numref:`fig-sky-color-layer`, the color layer (2) can be seen on black background.

.. _fig-sky-no-color-layer:
.. figure:: images/sky_no_color_layer.png
   :alt: Sky with raw background color and texture layer, but without color layer.

   Background sky color (1) and the alpha-blended sky texture layer (3) without the additively
   blended color layer (2).

.. _fig-sky-color-layer:
.. figure:: images/sky_color_layer_raw.png
   :alt: Color layer only.

   Additively blended color layer (2) only, rendered on black background.
