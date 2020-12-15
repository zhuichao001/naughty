yum -y install python3.6
yum -y install python3-pip

#pip3 install redis
wget 'https://files.pythonhosted.org/packages/b3/17/1e567ff78c83854e16b98694411fe6e08c3426af866ad11397cddceb80d3/redis-3.5.3.tar.gz'
tar xvzf redis-3.5.3.tar.gz
cd redis-3.5.3/
python3 setup.py install --prefix=/usr/

