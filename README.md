# rocm_smi_lib_rs
Crate bringing support for rocm-smi liblary from C

# Instalation

```
cargo add rocm_smi_lib
```

You can ommit instalation of rosm-smi by using `vendored` feature.


## To use this crate you have to install rocm-smi liblaries

## Manjaro
```
sudo pacman -S rocm-smi-lib rocm-smi
```

## Fedora / RHEL

Create file named `/etc/yum.repos.d/rocm.repo` and write it with:
```
[ROCm-6.0.2]
name=ROCm6.0.2
baseurl=https://repo.radeon.com/rocm/rhel9/6.0.2/main
enabled=1
priority=50
gpgcheck=1
gpgkey=https://repo.radeon.com/rocm/rocm.gpg.key
```
> For suse change file path to /etc/zypp/repos.d/rocm.repo `baseurl` to `https://repo.radeon.com/rocm/zyp/6.0.2/main` 

Then run 
On Fedora:
```
sudo dnf install rocm-smi rocm-smi-devel rocm-smi-lib
```
On RHEL:
```
sudo yum install --nogpgcheck rocblas rocm-smi-lib
```
On Suse:
```
sudo zypper install rocblas rocm-smi-lib
```

## Ubuntu 

Run commands:
```
echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/rocm.gpg] https://repo.radeon.com/rocm/apt/6.0.2 focal main" \
    | sudo tee --append /etc/apt/sources.list.d/rocm.list

echo -e 'Package: *\nPin: release o=repo.radeon.com\nPin-Priority: 600' \
    | sudo tee /etc/apt/preferences.d/rocm-pin-600

sudo apt update

sudo apt install rocm-dkms rocm-dev rocm-smi-lib
```

## Other
For other check out [How to install ROCM](https://rocm.docs.amd.com/projects/install-on-linux/en/latest/how-to/native-install/index.html)

# Usage / Examples

Example of printing the name of your GPU:
```rust
fn print_gpu_name() -> Result<(), RocmErr> {
    let rocm = RocmSmi::init()?;
    let name = rocm.get_device_identifiers(0)?.name;
    println!("{}", name);
    Ok(())
}    
```

Same thing as above but with creation of device object
```rust
fn print_gpu_name() -> Result<(), RocmErr> {
    let rocm = RocmSmi::init()?.into_first_device()?; // into first devic requires enabling `device` feature
    let name = rocm.get_identifiers()?.name;
    println!("{}", name);
    Ok(())
}    
```
