import matplotlib.pyplot as plt
import matplotlib.animation as animation
from datetime import datetime
import time
import sys
import os

# Import our C extension
try:
    import sensor_simulator as sensor
except ImportError:
    print("Error: C extension module not found.")
    print("Please compile it first: python setup.py build_ext --inplace")
    sys.exit(1)

class IoTDashboard:
    def __init__(self):
        # Initialize data storage
        self.timestamps = []
        self.temperatures = []
        self.humidities = []
        self.battery_levels = []
        self.start_time = time.time()
        
        # Create figure and subplots
        self.fig, ((self.ax1, self.ax2), (self.ax3, self.ax4)) = plt.subplots(2, 2, figsize=(12, 8))
        self.fig.suptitle('Real-time IoT Sensor Monitoring Dashboard', fontsize=16, fontweight='bold')
        
        # Initialize plots
        self.setup_plots()
        
    def setup_plots(self):
        # Temperature plot
        self.temp_line, = self.ax1.plot([], [], 'r-', linewidth=2, label='Temperature')
        self.ax1.set_title('Temperature Sensor')
        self.ax1.set_ylabel('Temperature (°C)')
        self.ax1.set_xlabel('Time (s)')
        self.ax1.grid(True, alpha=0.3)
        self.ax1.legend()
        self.ax1.set_ylim(15, 40)
        
        # Humidity plot
        self.humidity_line, = self.ax2.plot([], [], 'b-', linewidth=2, label='Humidity')
        self.ax2.set_title('Humidity Sensor')
        self.ax2.set_ylabel('Humidity (%)')
        self.ax2.set_xlabel('Time (s)')
        self.ax2.grid(True, alpha=0.3)
        self.ax2.legend()
        self.ax2.set_ylim(20, 100)
        
        # Battery level plot
        self.battery_line, = self.ax3.plot([], [], 'g-', linewidth=2, label='Battery Level')
        self.ax3.set_title('Battery Level')
        self.ax3.set_ylabel('Battery Level (%)')
        self.ax3.set_xlabel('Time (s)')
        self.ax3.grid(True, alpha=0.3)
        self.ax3.legend()
        self.ax3.set_ylim(0, 100)
        
        # Status display
        self.ax4.axis('off')
        self.status_text = self.ax4.text(0.1, 0.9, '', transform=self.ax4.transAxes, fontsize=12,
                                        verticalalignment='top', fontfamily='monospace')
        
        # Adjust layout
        plt.tight_layout()
        
    def update_plots(self, frame):
        # Get new sensor data from C extension
        sensor_data = sensor.get_sensor_readings()
        
        if sensor_data is None:
            print("Simulation completed (30 seconds elapsed)")
            return self.temp_line, self.humidity_line, self.battery_line, self.status_text
        
        # Extract data
        temp = sensor_data['temperature']
        humidity = sensor_data['humidity']
        battery = sensor_data['battery_level']
        count = sensor_data['count']
        current_time = time.time() - self.start_time
        
        # Store data
        self.timestamps.append(current_time)
        self.temperatures.append(temp)
        self.humidities.append(humidity)
        self.battery_levels.append(battery)
        
        # Keep only last 50 data points for performance
        if len(self.timestamps) > 50:
            self.timestamps = self.timestamps[-50:]
            self.temperatures = self.temperatures[-50:]
            self.humidities = self.humidities[-50:]
            self.battery_levels = self.battery_levels[-50:]
        
        # Update plot data
        self.temp_line.set_data(self.timestamps, self.temperatures)
        self.humidity_line.set_data(self.timestamps, self.humidities)
        self.battery_line.set_data(self.timestamps, self.battery_levels)
        
        # Adjust x-axis limits to show recent data
        for ax in [self.ax1, self.ax2, self.ax3]:
            if self.timestamps:
                ax.set_xlim(max(0, self.timestamps[0]), self.timestamps[-1] + 1)
        
        # Update status display
        status_info = f"""STATUS UPDATE:
Reading #: {count}
Time: {current_time:.1f}s
Temperature: {temp:.1f}°C
Humidity: {humidity:.1f}%
Battery: {battery:.1f}%
Last Update: {datetime.now().strftime('%H:%M:%S')}"""
        
        self.status_text.set_text(status_info)
        
        return self.temp_line, self.humidity_line, self.battery_line, self.status_text
    
    def start_monitoring(self):
        print("Starting IoT Monitoring Dashboard...")
        print("Simulation will run for 30 seconds")
        print("Press Ctrl+C to stop early")
        
        # Reset simulation
        sensor.reset_simulation()
        
        # Create animation
        self.ani = animation.FuncAnimation(
            self.fig, 
            self.update_plots, 
            interval=2000,  # Update every 2 seconds
            blit=True,
            cache_frame_data=False
        )
        
        plt.show()

def main():
    try:
        dashboard = IoTDashboard()
        dashboard.start_monitoring()
    except KeyboardInterrupt:
        print("\nMonitoring stopped by user")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()