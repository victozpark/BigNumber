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
$ cd build                   # Change to the build directory
$ cmake ..                   # Create a Makefile
$ make                       # Make a libray file
$ make install               # Install header and library files
$ ls -al ../release/include  # You can check a header file
$ ls -al ../release/lib      # You can check a library file
```

## Usage
```c++
vp::BigNumber bn01{"54321.001234"};
vp::BigNumber bn02{"56789.004321"};

// Addition
std::cout << bn01 + bn02 << std::endl; // Output : 111110.005555

// Subtraction
std::cout << bn01 - bn02 << std::endl; // Output : -2468.003087

// Multiplication
std::cout << bn01 * bn02 << std::endl; // Output : 3084835573.798672332114

// Division
std::cout << bn01 / bn02 << std::endl; // Output : 0.95654082834328973443

vp::BigNumber bn10{"76543.123456"};
vp::BigNumber bn11 = bn10;
vp::BigNumber bn12 = bn10;
vp::BigNumber bn13 = bn10;
vp::BigNumber bn14 = bn10;
vp::BigNumber bn21 = bn10;
vp::BigNumber bn22 = bn10;
vp::BigNumber bn23 = bn10;
vp::BigNumber bn24 = bn10;
vp::BigNumber bn31 = bn10;
vp::BigNumber bn32 = bn10;
vp::BigNumber bn33 = bn10;
vp::BigNumber bn34 = bn10;

// Round
std::cout << bn11.round(-5) << std::endl; // Output : 76543.1235
std::cout << bn12.round(-3) << std::endl; // Output : 76543.12
std::cout << bn13.round(3)  << std::endl; // Output : 77000
std::cout << bn14.round(1)  << std::endl; // Output : 76540

// Round up
std::cout << bn21.roundUp(-5) << std::endl; // Output : 76543.1235
std::cout << bn22.roundUp(-3) << std::endl; // Output : 76543.13
std::cout << bn23.roundUp(3)  << std::endl; // Output : 77000
std::cout << bn24.roundUp(1)  << std::endl; // Output : 76550

// Round down
std::cout << bn31.roundDown(-5) << std::endl; // Output : 76543.1234
std::cout << bn32.roundDown(-3) << std::endl; // Output : 76543.12
std::cout << bn33.roundDown(3)  << std::endl; // Output : 76000
std::cout << bn34.roundDown(1)  << std::endl; // Output : 76540
```

----
Please let me know if there are any bugs or features you would like to use. <br>
Thank you. :)
