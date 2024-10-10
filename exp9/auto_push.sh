#!/bin/bash
cd /home/cjj/hpc/experiment_9_task6
touch move.yml
cat head.yml >> move.yml
cat body.yml >> move.yml
cp move.yml /usr/local/prometheus/prometheus.yml
rm move.yml
echo 0311 | sudo -S systemctl daemon-reload
echo 0311 | sudo -S systemctl restart prometheus.service
echo 0311 | sudo -S systemctl restart node_exporter.service
