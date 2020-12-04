DEF_FUNCTION (cos, 1,
{
    val = cos (val);
})

DEF_FUNCTION (sin, 2,
{
    val = sin (val);
})

DEF_FUNCTION (tan, 3,
{
    double _old_val_ = val;
    val = cos (val);
    if (IsZero (val))
    {
        InvalidArgument (dep);
        return NAN;
    }
    val = sin (_old_val_)/val;
})
