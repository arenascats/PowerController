void linearSmooth5 ( double in[], double out[], int N )
{
    int i;
    if ( N < 5 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 3.0 * in[0] + 2.0 * in[1] + in[2] - in[4] ) / 5.0;
        out[1] = ( 4.0 * in[0] + 3.0 * in[1] + 2 * in[2] + in[3] ) / 10.0;
        for ( i = 2; i <= N - 3; i++ )
        {
            out[i] = ( in[i - 2] + in[i - 1] + in[i] + in[i + 1] + in[i + 2] ) / 5.0;
        }
        out[N - 2] = ( 4.0 * in[N - 1] + 3.0 * in[N - 2] + 2 * in[N - 3] + in[N - 4] ) / 10.0;
        out[N - 1] = ( 3.0 * in[N - 1] + 2.0 * in[N - 2] + in[N - 3] - in[N - 5] ) / 5.0;
    }
}
void cubicSmooth7(double in[], double out[], int N)
{
    int i;
    if ( N < 7 )
    {
        for ( i = 0; i <= N - 1; i++ )
        {
            out[i] = in[i];
        }
    }
    else
    {
        out[0] = ( 39.0 * in[0] + 8.0 * in[1] - 4.0 * in[2] - 4.0 * in[3] +
                  1.0 * in[4] + 4.0 * in[5] - 2.0 * in[6] ) / 42.0;
        out[1] = ( 8.0 * in[0] + 19.0 * in[1] + 16.0 * in[2] + 6.0 * in[3] -
                  4.0 * in[4] - 7.0* in[5] + 4.0 * in[6] ) / 42.0;
        out[2] = ( -4.0 * in[0] + 16.0 * in [1] + 19.0 * in[2] + 12.0 * in[3] +
                  2.0 * in[4] - 4.0 * in[5] + 1.0 * in[6] ) / 42.0;
        for ( i = 3; i <= N - 4; i++ )
        {
            out[i] = ( -2.0 * (in[i - 3] + in[i + 3]) +
                       3.0 * (in[i - 2] + in[i + 2]) +
                      6.0 * (in[i - 1] + in[i + 1]) + 7.0 * in[i] ) / 21.0;
        }
        out[N - 3] = ( -4.0 * in[N - 1] + 16.0 * in [N - 2] + 19.0 * in[N - 3] +
                      12.0 * in[N - 4] + 2.0 * in[N - 5] - 4.0 * in[N - 6] + 1.0 * in[N - 7] ) / 42.0;
        out[N - 2] = ( 8.0 * in[N - 1] + 19.0 * in[N - 2] + 16.0 * in[N - 3] +
                      6.0 * in[N - 4] - 4.0 * in[N - 5] - 7.0 * in[N - 6] + 4.0 * in[N - 7] ) / 42.0;
        out[N - 1] = ( 39.0 * in[N - 1] + 8.0 * in[N - 2] - 4.0 * in[N - 3] -
                      4.0 * in[N - 4] + 1.0 * in[N - 5] + 4.0 * in[N - 6] - 2.0 * in[N - 7] ) / 42.0;
    }
}
