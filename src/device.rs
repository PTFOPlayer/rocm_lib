use crate::{
    error::RocmErr,
    queries::{pcie::Pcie, power::Power},
    RocmSmi,
};

pub struct RocmSmiDevice {
    id: u32,
    rocm: RocmSmi,
}

impl RocmSmiDevice {
    #[inline(always)]
    pub(crate) fn new_from_rocm(rocm: RocmSmi, id: u32) -> Result<Self, RocmErr> {
        let dev_c = rocm.get_device_count()?;
        if id >= dev_c {
            return Err(RocmErr::RsmiStatusInputOutOfBounds);
        }
        Ok(Self { id, rocm })
    }

    pub fn new(id: u32) -> Result<Self, RocmErr> {
        let rocm = RocmSmi::init()?;
        let dev_c = rocm.get_device_count()?;
        if id >= dev_c {
            return Err(RocmErr::RsmiStatusInputOutOfBounds);
        }
        Ok(Self { id, rocm })
    }

    pub fn get_id(&self) -> Result<u16, RocmErr> {
        self.rocm.get_device_id(self.id)
    }

    pub fn get_name(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_name(self.id)
    }

    pub fn get_vemdor_id(&self) -> Result<u16, RocmErr> {
        self.rocm.get_device_vendor_id(self.id)
    }

    pub fn get_brand(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_brand(self.id)
    }

    pub fn get_vendor_name(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_vendor_name(self.id)
    }

    pub fn get_vram_vendor_name(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_vram_vendor_name(self.id)
    }

    pub fn get_serial_number(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_serial_number(self.id)
    }

    pub fn get_subsystem_id(&self) -> Result<u16, RocmErr> {
        self.rocm.get_device_subsystem_id(self.id)
    }

    pub fn get_subsystem_name(&self) -> Result<String, RocmErr> {
        self.rocm.get_device_subsystem_name(self.id)
    }

    pub fn get_drm_render_minor(&self) -> Result<u32, RocmErr> {
        self.rocm.get_device_drm_render_minor(self.id)
    }

    pub fn get_subsystem_vendor_id(&self) -> Result<u16, RocmErr> {
        self.rocm.get_device_subsystem_vendor_id(self.id)
    }

    pub fn get_unique_id(&self) -> Result<u64, RocmErr> {
        self.rocm.get_device_unique_id(self.id)
    }

    pub fn get_pcie_data<'a>(&self) -> Result<Pcie<'a>, RocmErr> {
        self.rocm.get_device_pcie_data(self.id)
    }

    pub fn get_power_data(&self) -> Result<Power, RocmErr> {
        self.rocm.get_device_power_data(self.id)
    }
}