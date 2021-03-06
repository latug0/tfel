The `@ModellingHypothesis` keyword allows the user to choose the
modelling hypothesis to use. The keywords shall be one of the first
keyword of the file: if not, some keywords, which requires the
modelling hypothesis to be defined, may force the default modelling
hypothesis to be used (`Tridimensional`).

This keyword is followed by the desired modelling hypothesis given as
a string. The following values are currently supported:

- AxisymmetricalGeneralisedPlaneStrain (1D)
- Axisymmetrical         (2D)
- PlaneStress            (2D)
- PlaneStrain            (2D)
- GeneralisedPlaneStrain (2D)
- Tridimensional         (3D)

The modelling hypothesis changes the name of the components of
symmetric tensors and their numbers. For example, the components of
the strains are:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY EZZ EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ
