__kernel void calcPi(global unsigned long *startingPoints,
                     global double *outputData, global unsigned long *num) {
  int id = get_global_id(0) - 1;
  unsigned long c = startingPoints[id];
  double numerator = 4.0;
  double denominator = 1.0 + (c * 2.0);
  double operation = (c % 2 == 1) ? -1.0 : 1.0;
  double pi = 0.0;
  unsigned long dif = *num;
  for (unsigned long i = 0; i < dif; i++) {
    pi += operation * (numerator / denominator);
    denominator += 2.0;
    operation *= -1.0;
  }

  outputData[id] = pi;
}