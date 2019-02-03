# REGoth-bs
REGoth using bs::f

# Documentation

https://regoth-project.github.io/REGoth-bs/index.html

# Building

## Engine

(TODO)

## Building the Documentation

You will need:

 * Doxygen
 * Python
  * Sphinx (install via `pip install sphinx`)
  * Breathe (install via `pip install breathe`)
  * Sphinx (install via `pip install sphinx_rtd_theme`)

With a cmake-project generated, run this from within the `build`-directory:

```sh
cmake --build . --target docs
```

This will generate the documentation as HTML into `build/docs-source/html`. 
To update Github-pages, copy the contents of that directory into the `docs` directory at the repository root.
