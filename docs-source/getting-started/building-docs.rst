Building the documentation
==========================

.. note::

  You can skip this section if you just want to get REGoth running.  This guide is only relevant
  if you want to contribute to this documentation.

This documentation is written as reStructuredText and built using
`Sphinx <http://www.sphinx-doc.org>`_.
See `here <http://docutils.sourceforge.net/docs/user/rst/quickref.html>`_
for a quick reference of the ``rst`` syntax.  You will also need to have Doxygen installed (see
:ref:`building_regoth`, Development dependencies).


pip packages
------------

The following ``pip`` packages are required:

.. code-block:: sh

  $ pip install sphinx breathe sphinx_rtd_theme sphinxcontrib-plantuml

As usual with Python, it is recommended to use a virtual environment.


Building
--------

With a CMake project generated, run this from within the ``build`` directory:

.. code-block:: sh

  $ cmake --build . --target REGoth_docs

This will generate the documentation as HTML into ``build/docs-source/html``. Just open
``build/docs-source/html/index.html`` in your browser to view the documentation locally.  To update
GitHub-pages, copy the contents of that directory into the ``docs`` directory at the repository
root.


Modifying an existing chapter
-----------------------------

Simply go to ``docs-source/content`` and open one of the ``.rst``-files with a text editor of your
choice.


Adding a new chapter
--------------------

To add a new chapter you will have to:

1. Create a new ``.rst``-file: ``docs-source/content/<your-chapter>.rst``
2. Add the new ``.rst``-file to ``docs-source/index.rst``
