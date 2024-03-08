make
ips="192.168.134.83 192.168.134.84"
for ip in $ips;do
    rsync -rucalpvz . SDS_Admin@$ip:/home/SDS_Admin/xb/rdma/rdma-file-transfer/
    echo -e "cd /home/SDS_Admin/xb/rdma/rdma-file-transfer/"
done