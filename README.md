# Big number

## Purposes
* To calculate large numbers.
* To provide high floating point precision. (To solve machine epsilon problem)
     
## Precautions
#### Supported operations and functions inlcude:
  - Four fundamental arithmetic operations (+, -, *, /)
  - Comparison operators (!=, ==, >, >=, <, <=)
  - Other operations (+=, -=, *=, /=)
  - Round, RoundUp, RoundDown functions.
  
#### Maximum length of the fractional part.
  - Default value is 20.
  - This can be set via one of constructor functions or BigNumber::setMaxFracLen function.
  - If the maximum lenth of fractional part is less than the calulated result, the result will be rounded at the maximum lenth of fractional part + 1 position.

## Limits
#### The maximum numeric string length is 2^31 - 1.
   > #### A numeric string consists of :
   > - One or less than one sign character (+/-) in the first numeric string.
   > - One or less than one decimal point between numeric characters.
   > - Numeric characters.

----

## Install
```shell
$ mkdir build                # Make a build directory
$ cmake ..                   # Create a Makefile
$ make                       # Make a libray file
$ make install               # Install library and a header files
$ ls -al ../release/include  # You can check a header file
$ ls -al ../release/lib      # You can check a library file
```

## Usage
```c++
vp::BigNumber bn01{"54321.001234"};
vp::BigNumber bn02{"56789.004321"};

// ADD
std::cout << bn01 + bn02 << std::endl; // Output : 111110.005555

// MINUS
std::cout << bn01 - bn02 << std::endl; // Output : -2468.003087

// MULTIPLY
std::cout << bn01 * bn02 << std::endl; // Output : 3084835573.798672332114

// DIVIDE
std::cout << bn01 / bn02 << std::endl; // Output : 0.95654082834328973443
```

----
Please let me know if there are any bugs or features you would like to use. <br>
Thank you. :)
