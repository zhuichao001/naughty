# Advisory locking
types:  
  1.BSD locks (flock)  
  2.POSIX record locks (fcntl, lockf)  
  3.Open file description locks (fcntl)  
Common features:  
  1.All locks support blocking and non-blocking operations.  
  2.Locks are allowed only on files, but not directories.  
  3.Locks are automatically removed when the process exits or terminates.  

# Mandatory locking
