#include <Arduino.h>

class MillisStepper
{
  private: unsigned long lastRun = 0;
  private: unsigned long lastAcceleration = 0;
  private: unsigned int motorFullRevolutionSteps = 0;
  private: unsigned long stepInterval = 0;
  private: unsigned int motorRPMTarget = 0;
  private: unsigned int motorRPMCurrent = 0;
  private: unsigned const long oneMinuteDuration = 60000000;
  private: bool accelerationEnabled = true;
  private: bool motorActive = false;
  private: bool motorClockwise = true;
  private: bool motorMagnetsStillOn = false;
  private: byte stepProgress = 0;
  private: byte phase1A = 0;
  private: byte phase1B = 0;
  private: byte phase2A = 0;
  private: byte phase2B = 0;
  public: const String version = "1.0.0";


  public: MillisStepper(byte phase1A, byte phase1B, byte phase2A, byte phase2B) {
    this->phase1A = phase1A;
    this->phase1B = phase1B;
    this->phase2A = phase2A;
    this->phase2B = phase2B;
    this->doPinSetup();
    this->setOutputs(LOW, LOW, LOW, LOW);
  }


  private: void doPinSetup() {
    pinMode(this->phase1A, OUTPUT);
    pinMode(this->phase1B, OUTPUT);
    pinMode(this->phase2A, OUTPUT);
    pinMode(this->phase2B, OUTPUT);
  }


  private: void setOutputs(bool phase1A, bool phase1B, bool phase2A, bool phase2B) {
    digitalWrite(this->phase1A, phase1A);
    digitalWrite(this->phase1B, phase1B);
    digitalWrite(this->phase2A, phase2A);
    digitalWrite(this->phase2B, phase2B);
  }


  public: void setFullRevSteps(unsigned int b) {
    this->motorFullRevolutionSteps = b;
  }


  public: int getFullRevSteps() {
    return this->motorFullRevolutionSteps;
  }


  public: void setClockwise(bool b){
    if (this->motorClockwise != b)
      this->stepProgress = 0;
    this->motorClockwise = b;
  }


  public: bool getClockwise() {
    return this->motorClockwise;
  }


  public: void setActive(bool b) {
    this->motorActive = b;
  }


  public: bool getActive() {
    return this->motorActive;
  }


  public: void setRPM(unsigned int i) {
    if (this->motorRPMCurrent == i || (this->accelerationEnabled && this->motorRPMTarget == i )) return;
    
    if (this->accelerationEnabled) {
      this->motorRPMTarget = i;
    } else {
      this->motorRPMCurrent = i;
      this->stepInterval = this->oneMinuteDuration / (this->motorFullRevolutionSteps * this->motorRPMCurrent);
    }
  }


  public: int getRPM() {
    return this->motorRPMCurrent;
  }


  public: void doRun() {
    bool accelerationNecessary = this->accelerationEnabled && (this->motorRPMCurrent != this->motorRPMTarget);
    
    if (!accelerationNecessary && (this->motorRPMCurrent == 0 || !this->motorActive)) {
      if (this->motorMagnetsStillOn) {
        this->setOutputs(LOW, LOW, LOW, LOW);
        this->motorMagnetsStillOn = false;
      }
    } else {
      this->motorMagnetsStillOn = true;

      if (accelerationNecessary) {
        unsigned long waitingTime = millis() - this->lastAcceleration;
        unsigned int rpm = this->motorRPMCurrent;

        if (waitingTime > 20) {
          if (this->motorRPMTarget < this->motorRPMCurrent) {
            rpm -= 1;
          } else {
            rpm += 1;
          }

          this->accelerationEnabled = false;
          this->setRPM(rpm);
          this->accelerationEnabled = true;

          this->lastAcceleration = millis();
        }
      }
      
      unsigned long waitingTime = micros() - this->lastRun;

      if (waitingTime > this->stepInterval) {
        this->doStep();
        this->lastRun = micros();
      }
    }
  }


  public: void doStep() {
    if (this->stepProgress == 0) {
      if (this->motorClockwise)
        this->setOutputs(LOW, LOW, LOW, HIGH);
      else
        this->setOutputs(HIGH, LOW, LOW, LOW);
    }

    else if (this->stepProgress == 1) {
      if (this->motorClockwise)
        this->setOutputs(LOW, HIGH, LOW, LOW);
      else
        this->setOutputs(LOW, LOW, HIGH, LOW);
    }

    else if (this->stepProgress == 2) {
      if (this->motorClockwise)
        this->setOutputs(LOW, LOW, HIGH, LOW);
      else
        this->setOutputs(LOW, HIGH, LOW, LOW);
    }
    
    else if (this->stepProgress == 3) {
      if (this->motorClockwise)
        this->setOutputs(HIGH, LOW, LOW, LOW);
      else
        this->setOutputs(LOW, LOW, LOW, HIGH);
    }

    if (this->stepProgress < 3) {
      this->stepProgress += 1;
    } else {
      this->stepProgress = 0;
    }
  }
};
