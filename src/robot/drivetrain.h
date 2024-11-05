#pragma once

namespace Robot {
  class Drivetrain {
    public:
      Drivetrain();
      enum DriveMode { ARCADE = 0, TANK = 1};
      static DriveMode driveMode;

      void run();

      static void setDriveMode(DriveMode mode);
      static void toggleDriveMode();
    private:
      void arcade();
      void tank();    
  };
}
