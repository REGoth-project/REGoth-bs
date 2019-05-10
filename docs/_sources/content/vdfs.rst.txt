Virtual File System
===================

Gothic stores it's files in something called a *Virtual File System*, or short *VDFS*. It is built out of multiple packages,
which are all loaded into one file index. Think of it as putting all textures, meshes and other content into different ZIP-Archives
and have the engine merge them all during load-time. 

The package format used by Gothic is *not* ZIP, but an uncompressed proprietary format. We won't get into details about how that 
format is structured here.

Creating a VDFS-File index
--------------------------

To do anything with this library, you need to populate a Virtual File Index with the packages from the original game, which is rather
straight forward:

First, you need to include the VDFS-module from ZenLib:

.. code-block:: cpp

    #include <vdfs/fileIndex.h>

In `main`, you then need to initialize the VDFS with `argv[0]`, which is the path to the running executable. This is a detail of the underlaying PhysFS library.

.. code-block:: cpp

    VDFS::FileIndex::initVDFS(argv[0]);

Now you can create a FileIndex and load some packages from your games `DATA`-folder into it:

.. code-block:: cpp

    const std::string dataDir = "D:/Games/Gothic II/Data";

    VDFS::FileIndex vdfs;
    vdfs.loadVDF(dataDir + "/Meshes.vdf");
    vdfs.loadVDF(dataDir + "/Textures.vdf");
    vdfs.loadVDF(dataDir + "/Anims.vdf");
    vdfs.finalizeLoad();

With the file index populated, you can now access the files stored inside the packages:

.. code-block:: cpp

    for(std::string file : vdf.getKnownFiles())
    {
        std::cout << file << std::endl;
    }

The full example:

.. code-block:: cpp

    #include <vdfs/fileIndex.h>
    #include <iostream>

    int main(int argc, char** argv)
    {
        VDFS::FileIndex::initVDFS(argv[0]);

        const std::string dataDir = "D:/Games/Gothic II/Data";

        VDFS::FileIndex vdfs;
        vdfs.loadVDF(dataDir + "/Meshes.vdf");
        vdfs.loadVDF(dataDir + "/Textures.vdf");
        vdfs.loadVDF(dataDir + "/Anims.vdf");
        vdfs.finalizeLoad();

        for(std::string file : vdf.getKnownFiles())
        {
            std::cout << file << std::endl;
        }

        return 0;
    }


Importing content using a FileIndex
-----------------------------------

A populated VDFS FileIndex can now be used to import files, as an example, this is how one would import a texture:

.. code-block:: cpp

    HTexture wood = BsZenLib::ImportTexture("WOOD.TGA", vdfs);

The a file matching `WOOD.TGA` must be registered inside the passed VDFS FileIndex.

.. note:: The Texture-Importer searches for `WOOD.TGA` *and* `WOOD-C.TEX` automatically.