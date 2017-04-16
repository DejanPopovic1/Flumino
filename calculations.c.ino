#define MICROSECONDS_PER_MILLISECOND 1000
#define MILLISECONDS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MINUTE_PER_HOUR 60

const float alpha = 0.25;
const int maximumDebouncePeriod = 20;

static double averageFlowRate(double instantaneousFlowRate, double *previouslyAveragedFlowRate){
  return (double)alpha * instantaneousFlowRate + (1 - alpha) * *previouslyAveragedFlowRate;
}

double flowRate(unsigned long *const timeStart, const unsigned long timeEnd, const int dropsPerMillilitre, double *const previouslyAveragedFlowRate, unsigned long *const period){  
  *period = timeEnd - *timeStart;
  double dropsPerHour;
  double instantaneousFlowRate;
  if (*period < maximumDebouncePeriod){
    ;
  }
  else{
    dropsPerHour = ((double)MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE * MINUTE_PER_HOUR) / *period;
    instantaneousFlowRate = dropsPerHour / dropsPerMillilitre;
    *timeStart = timeEnd;
    return averageFlowRate(instantaneousFlowRate, previouslyAveragedFlowRate);
  }
}

double decayedFlowRate(const unsigned long timeStart, const unsigned long timeEnd, const int dropsPerMillilitre, unsigned long period){  
  period = timeEnd - timeStart;
  double dropsPerHour;
  double instantaneousFlowRate;
  if (period < maximumDebouncePeriod){
    ;
  }
  else{
    dropsPerHour = ((double)MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE * MINUTE_PER_HOUR) / period;
    return instantaneousFlowRate = dropsPerHour / dropsPerMillilitre;
  }
}