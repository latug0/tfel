The `@InternalStateVariable` keyword defines the inital value of a
state variable.

This keyword must be followed by the name of the internal state
variable considered, given as a string.

If this internal state variable is scalar, a real value is then
expected.

If this internal state variable is a symmetric tensor, an array of
real values of the appropriate size is expected. A symmetric tensor
has 3 components in 1D, 4 components in 2D and 6 components in 3D (see
the `@ModellingHypothesis` keyword). The off-diagonals components
shall be declared with a \(\sqrt(2)\) factor.

## Example

~~~~ {.cpp}
@InternalStateVariable 'ElasticStrain' {'EELRR0','EELZZ0','EELZZ0',0.};
~~~~~~~~
