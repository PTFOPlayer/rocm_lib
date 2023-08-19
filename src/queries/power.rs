use crate::{bindings::*, error::RocmErr};

#[derive(Debug, Clone, Copy)]
pub struct PowerCapRange {
    pub min: f64,
    pub max: f64,
}

#[derive(Debug, Clone)]
pub struct Power {
    pub sensor_count: u16,
    pub power_avg: f64,
    pub power_per_sensor: Vec<f64>,
    pub current_sensor_power_cap: Vec<f64>,
    pub default_power_cap: f64,
    pub power_cap_range: PowerCapRange,
}

pub(crate) fn get_sensors(dev_id: u32) -> Result<u16, RocmErr> {
    let sensors = unsafe { power_sensor_count(dev_id) };
    check_res(sensors.status)?;
    Ok(sensors.data)
}

impl Power {
    pub(crate) unsafe fn get_power(dev_id: u32) -> Result<Power, RocmErr> {
        let sensors = power_sensor_count(dev_id);
        check_res(sensors.status)?;

        let power_per_sensor = {
            let mut power_avg_sensor = vec![] ;
            for s in 0..sensors.data {
                let temp = power_sensor(dev_id, s);
                check_res(temp.status)?;
                power_avg_sensor.push( temp.data as f64 / 1000.);
            }
            power_avg_sensor
        };

        let avg_power = power_avg_all(dev_id);
        check_res(avg_power.status)?;

        let current_sensor_power_cap = {
            let mut power_sensor_cap = vec![] ;
            for s in 0..sensors.data {
                let temp = power_cap(dev_id, s);
                check_res(temp.status)?;
                power_sensor_cap.push(temp.data as f64 / 1000.);
            }
            power_sensor_cap
        };

        let default_power_cap = default_power_cap(dev_id);
        check_res(default_power_cap.status)?;

        let power_cap_range = power_cap_range(dev_id);
        check_res(power_cap_range.status)?;

        Ok(Power {
            sensor_count: sensors.data,
            power_avg: avg_power.data as f64 / 1000.,
            power_per_sensor,
            current_sensor_power_cap,
            default_power_cap: default_power_cap.data as f64 / 1000.,
            power_cap_range: PowerCapRange {
                min: power_cap_range.data1 as f64 / 1000.,
                max: power_cap_range.data2 as f64 / 1000.,
            },
        })
    }
}
