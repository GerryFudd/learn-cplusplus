## Setting up your IDE

### VS Code with plugin

VS Code seems to be set up well for c++ development. Download the C/C++ plugin and make sure that your `./includes` directory is set under the `C_Cpp > Default: Include Path` setting. This will tell the intellisense detector where you import your files from so that you can get accurate feedback on missing dependencies and compile errors as you write your code.

### Setting up tests

The standard unit testing library for c++ is Boost.test. [Download the latest version of Boost](https://www.boost.org/users/download/) (I downloaded the `.tar.gz` file when I initially set up the boost library). Unpack the download file and navigate to the resulting directory in your terminal. Then run te bootstrap script followed by the b2 script and specify where you want to keep your boost test dependencies. I put mine into `/usr/local/boost`, which required admin permission.

```zsh
% ./bootstrap.sh
% sudo ./b2 --with-test --prefix=/usr/local/boost install
```

This creates two directories, `/usr/local/boost/include/` and `/usr/local/boost/lib/` that contain all of the relevant header and source files from the boost test library. Now add `/usr/local/boost/include/` to `C_Cpp > Default: Include Path` and you will be able to include boost dependencies.
