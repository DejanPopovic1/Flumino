#define MICROSECONDS_PER_MILLISECOND 1000
#define MILLISECONDS_PER_SECOND 1000
#define SECONDS_PER_MINUTE 60
#define MINUTE_PER_HOUR 60

const float alpha = 0.25;
const int maximumDebouncePeriod = 20;

static double averageFlowRate(double instantaneousFlowRate, double previouslyAveragedFlowRate) {
  return (double)alpha * instantaneousFlowRate + (1 - alpha) * previouslyAveragedFlowRate;
}

double flowRate(struct MachineState *s) {
  s->period = s->currentTime - s->previousTime;
  double dropsPerHour;
  double instantaneousFlowRate;
  if (s->period < maximumDebouncePeriod) {
    ;
  }
  else {
    dropsPerHour = ((double)MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE * MINUTE_PER_HOUR) / s->period;
    instantaneousFlowRate = dropsPerHour / s->dropsPerMillilitre;
    s->previousTime = s->currentTime;
    return averageFlowRate(instantaneousFlowRate, s->newFlowRate);
  }
}

double decayedFlowRate(const unsigned long timeStart, const unsigned long timeEnd, const int dropsPerMillilitre, unsigned long period) {
  period = timeEnd - timeStart;
  double dropsPerHour;
  double instantaneousFlowRate;
  if (period < maximumDebouncePeriod) {
    ;
  }
  else {
    dropsPerHour = ((double)MICROSECONDS_PER_MILLISECOND * MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE * MINUTE_PER_HOUR) / period;
    return instantaneousFlowRate = dropsPerHour / dropsPerMillilitre;
  }
}

double drugFlowRate(struct MachineState *s) {
  if (s->inputDrugMassUOMSelector == 0 && s->dose_shown == 0) {
    s->DrugFlowRate = (s->drugMassUgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector) * 1000 ;
  }
  else if (s->inputDrugMassUOMSelector == 0 && s->dose_shown == 1){
    s->DrugFlowRate = (s->drugMassUgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector);
  }
  else if (s->inputDrugMassUOMSelector == 0 && s->dose_shown == 2){
    s->DrugFlowRate = (s->drugMassUgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector) / 1000;
  }
  else if (s->inputDrugMassUOMSelector == 1 && s->dose_shown == 0){
    s->DrugFlowRate = (s->drugMassMgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector) * 1000000;
  }
  else if (s->inputDrugMassUOMSelector == 1 && s->dose_shown == 1){
    s->DrugFlowRate = (s->drugMassMgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector) * 1000;
  }
  else if (s->inputDrugMassUOMSelector == 1 && s->dose_shown == 2){
    s->DrugFlowRate = (s->drugMassMgSelector * s->DisplayedFlowRate) / (s->volumeDilutantSelector * 60 * s->patientMassSelector);
  }
}
