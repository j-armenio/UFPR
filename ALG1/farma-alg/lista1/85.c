#include <stdio.h>

void main()
{
    double diametro, raio, volume;

    scanf("%lf", &diametro);

    raio = diametro / 2;

    volume = ((4 * 3.14) / 3) * (raio * raio * raio);

    printf("%lf", volume);
}