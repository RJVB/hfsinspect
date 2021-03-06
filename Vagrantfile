# -*- mode: ruby -*-
# vi: set ft=ruby :

$provision_script = <<SCRIPT
SETUPFLAG="/var/.bootstrapped"
if [ ! -e $SETUPFLAG ]
  then
  if [ -d /etc/apt ]; then
    # Fetch updated packages
    aptitude -yq update

    # Leave the kernel and boot code alone
    aptitude hold grub-common grub-pc grub-pc-bin grub2-common
    aptitude hold initramfs-tools initramfs-tools-bin
    aptitude hold linux-virtual linux-firmware linux-server
    aptitude hold linux-image-virtual linux-image-server
    aptitude hold linux-headers-virtual linux-headers-generic linux-headers-server

    # Upgrade everything else
    aptitude -yq safe-upgrade

    # Install developent tools
    aptitude -yq install build-essential clang pkg-config uuid-dev libbsd-dev
  fi
  
  # Link /vagrant to somewhere useful
  ln -s /vagrant ~vagrant/hfsinspect
  
  touch $SETUPFLAG
fi
SCRIPT

Vagrant.configure("2") do |config|
  # If you aren't already doing this ... why not?
  config.vm.synced_folder "~/.vagrant.d/caches/apt", "/var/cache/apt/archives"
  config.vm.synced_folder "~/.vagrant.d/caches/pip", "/home/vagrant/.pip_download_cache"

  config.vm.define :linux32 do |linux32|
    linux32.vm.box = "ubuntu/trusty32"
    linux32.vm.network :private_network, ip: "10.8.8.10"
    
    linux32.vm.provision "shell", inline: $provision_script
  
    linux32.vm.provider "virtualbox" do |v|
      v.customize ["setextradata", :id, "VBoxInternal/CPUM/SSE4.1", "1"]
      v.customize ["setextradata", :id, "VBoxInternal/CPUM/SSE4.2", "1"]
    end
  end

  config.vm.define :linux64 do |linux64|
    linux64.vm.box = "ubuntu/trusty64"
    linux64.vm.network :private_network, ip: "10.8.8.11"
    
    linux64.vm.provision "shell", inline: $provision_script
  
    linux64.vm.provider "virtualbox" do |v|
      v.customize ["setextradata", :id, "VBoxInternal/CPUM/SSE4.1", "1"]
      v.customize ["setextradata", :id, "VBoxInternal/CPUM/SSE4.2", "1"]
    end
  end
  
  # config.vm.define :freebsd64 do |freebsd64|
  #   freebsd64.vm.box = "freebsd_10.1_amd64_wunki"
  #   freebsd64.vm.box_url = "http://files.wunki.org/freebsd-10.1-amd64-wunki.box"
  #   freebsd64.vm.network :private_network, ip: "10.8.8.12"
  #
  #   # use NFS for the synced folder
  #   freebsd64.vm.synced_folder ".", "/vagrant", :nfs => true
  # end
  
end
