# Advisory locking
* Types:  
  1. BSD locks (flock)  
  2. POSIX record locks (fcntl, lockf)  
  3. Open file description locks (fcntl)  

* Conclution:  
  (a). BSD locks allow thread synchronization but don’t allow atomic mode switch.  
  (b). POSIX record locks don’t allow thread synchronization but allow atomic mode switch.  
  (c). Open file description locks allow both but are available only on recent Linux kernels.  

* Common features:  
  1. All locks support blocking and non-blocking operations.  
  2. Locks are allowed only on files, but not directories.  
  3. Locks are automatically removed when the process exits or terminates.  

# Mandatory locking
* Conclution(must at the same time):  
  1. The partition was mounted with the mand option.
  2. The set-group-ID bit is on and group-execute bit is off for the file.
  3. A POSIX record lock is acquired.
