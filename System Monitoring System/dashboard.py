import matplotlib.pyplot as plt
import matplotlib.animation as animation
from datetime import datetime
import time
import sensor_simulator

class IoTDashboard:
    def __init__(self):
        self.timestamps = []
        self.temperatures = []
        self.humidities = []
        self.battery_levels = []
    
        self.fig, (self.ax1, self.ax2, self.ax3) = plt.subplots(3, 1, figsize=(12, 10))
        self.fig.suptitle('Real Time IoT Sensor Dashboard', fontsize=16, fontweight='bold')
        
        self.setup_plots()
        
        sensor_simulator.reset_simulation()
        
    def setup_plots(self):
        self.ax1.clear()
        self.ax1.set_ylabel('Temperature (°C)', fontweight='bold')
        self.ax1.set_ylim(15, 35)
        self.ax1.grid(True, alpha=0.3)
        self.ax1.set_title('Temperature Sensor', fontweight='bold')
        self.temp_line, = self.ax1.plot([], [], 'r-', linewidth=2, label='Temperature')
        self.ax1.legend()
        
        self.ax2.clear()
        self.ax2.set_ylabel('Humidity (%)', fontweight='bold')
        self.ax2.set_ylim(30, 90)
        self.ax2.grid(True, alpha=0.3)
        self.ax2.set_title('Humidity Sensor', fontweight='bold')
        self.humidity_line, = self.ax2.plot([], [], 'b-', linewidth=2, label='Humidity')
        self.ax2.legend()
        
        self.ax3.clear()
        self.ax3.set_ylabel('Battery Level (%)', fontweight='bold')
        self.ax3.set_xlabel('Time (seconds)', fontweight='bold')
        self.ax3.set_ylim(0, 110)
        self.ax3.grid(True, alpha=0.3)
        self.ax3.set_title('Battery Level', fontweight='bold')
        self.battery_line, = self.ax3.plot([], [], 'g-', linewidth=2, label='Battery')
        self.ax3.legend()
        
        plt.tight_layout()
        
    def update_plots(self, frame):
        try:
            sensor_data = sensor_simulator.get_sensor_readings()
            
            timestamp = sensor_data['timestamp']
            temperature = sensor_data['temperature']
            humidity = sensor_data['humidity']
            battery_level = sensor_data['battery_level']
            
            self.timestamps.append(timestamp)
            self.temperatures.append(temperature)
            self.humidities.append(humidity)
            self.battery_levels.append(battery_level)
            
            if len(self.timestamps) > 15:  
                self.timestamps = self.timestamps[-15:]
                self.temperatures = self.temperatures[-15:]
                self.humidities = self.humidities[-15:]
                self.battery_levels = self.battery_levels[-15:]
            
            self.ax1.clear()
            self.ax1.plot(self.timestamps, self.temperatures, 'r-', linewidth=2, label='Temperature')
            self.ax1.set_ylabel('Temperature (°C)', fontweight='bold')
            self.ax1.set_ylim(15, 35)
            self.ax1.grid(True, alpha=0.3)
            self.ax1.set_title(f'Temperature Sensor - Current: {temperature:.1f}°C', fontweight='bold')
            self.ax1.legend()
            
            self.ax2.clear()
            self.ax2.plot(self.timestamps, self.humidities, 'b-', linewidth=2, label='Humidity')
            self.ax2.set_ylabel('Humidity (%)', fontweight='bold')
            self.ax2.set_ylim(30, 90)
            self.ax2.grid(True, alpha=0.3)
            self.ax2.set_title(f'Humidity Sensor - Current: {humidity:.1f}%', fontweight='bold')
            self.ax2.legend()
            
            self.ax3.clear()
            self.ax3.plot(self.timestamps, self.battery_levels, 'g-', linewidth=2, label='Battery')
            self.ax3.set_ylabel('Battery Level (%)', fontweight='bold')
            self.ax3.set_xlabel('Time (seconds)', fontweight='bold')
            self.ax3.set_ylim(0, 110)
            self.ax3.grid(True, alpha=0.3)
            self.ax3.set_title(f'Battery Level - Current: {battery_level:.1f}%', fontweight='bold')
            self.ax3.legend()
            
            current_time = datetime.now().strftime("%H:%M:%S")
            self.fig.suptitle(f'Real-Time IoT Sensor Monitoring Dashboard - {current_time}', 
                            fontsize=16, fontweight='bold')
            
            plt.tight_layout()
            
            if timestamp >= 30:
                print("Simulation completed 30 seconds of data collection")
                return
            
        except Exception as e:
            print(f"Error updating plots: {e}")
        
        return self.temp_line, self.humidity_line, self.battery_line
    
    def start_monitoring(self):
        print("Starting IoT Sensor Monitoring...")
        print("Simulation will run for 30 seconds (15 data points at 2-second intervals)")
        print("Close the plot window to stop early")
        
        ani = animation.FuncAnimation(
            self.fig, 
            self.update_plots, 
            interval=2000,  
            blit=False,
            cache_frame_data=False
        )
        
        plt.show()

def main():
    try:
        print("Testing C extension module...")
        sensor_simulator.reset_simulation()
        
        sample_data = sensor_simulator.get_sensor_readings()
        print(f"Sample sensor reading: {sample_data}")
        
        dashboard = IoTDashboard()
        dashboard.start_monitoring()
        
    except ImportError:
        print("Error")
        print("python setup.py")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()