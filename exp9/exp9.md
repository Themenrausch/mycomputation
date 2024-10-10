# exp9

## 方案:

**环境**:采用**Node Exporter**采集主机数据，**Prometheus**周期性的从**Exporter**暴露的**HTTP**服务地址（通常是/metrics）拉取监控样本数据，收集、存储数据并且对外提供数据查询支持。**Grafana**对**Prometheus**时序数据库中的数据进行可视化。

**个性化配置**:利用**shell**脚本(**auto_push**.*sh*)根据输入的主机**ip**地址修改**Prometheus**的**prometheus**.*yml*配置文件，使得**Prometheus**能够接收新的主机的数据。同时**shell**脚本通过一段**python**脚本(**read**.*py*)生成新的*.json*文件使得**Grafana**能够根据这段新的*.json*文件生成新的可视化图形。

## 详细的配置说明：

**node_exporter, version 1.8.2**

**prometheus-3.0.0-beta.0.linux**

**prometheus**.*yml*:

```yaml
# my global config
global:
  scrape_interval: 15s # Set the scrape interval to every 15 seconds. Default is every 1 minute.
  evaluation_interval: 15s # Evaluate rules every 15 seconds. The default is every 1 minute.
  # scrape_timeout is set to the global default (10s).

# Alertmanager configuration
alerting:
  alertmanagers:
    - static_configs:
        - targets:
          # - alertmanager:9093

# Load rules once and periodically evaluate them according to the global 'evaluation_interval'.
rule_files:
  # - "first_rules.yml"
  # - "second_rules.yml"

# A scrape configuration containing exactly one endpoint to scrape:
# Here it's Prometheus itself.
scrape_configs:
  # The job name is added as a label `job=<job_name>` to any timeseries scraped from this config.
  

  - job_name: 'node_exporter1'
    static_configs:
      - targets: [10.249.12.89:9100] # 连接服务器集群ip，获取对应的node_exporter数据

  - job_name: 'node_exporter2'
    static_configs:
      - targets: [localhost:9100] # chm

  - job_name: 'node_exporter3'
    static_configs:
      - targets: [172.20.4.37:9100] # cjj
```

**grafana-enterprise 11.2.2**

**使用的shell脚本**：

```shell
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
```

**使用的python脚本**:

```python
import json
import sys

name = sys.argv[1]

file_path_in = "./grafana_proto.json"
 
with open(file_path_in, 'r') as file:
    dd = json.load(file)
file.close()

file_path_txt = "./cal.txt"
with open(file_path_txt, 'r') as file:
    num = int(file.read())
file.close()
num += 1
with open(file_path_txt,"w") as f:
    f.write(str(num))

dd['panels'][0]['title'] = name
dd['panels'][0]['gridPos']['y'] = num * 9

dd['panels'][1]['targets'][0]['expr'] = 'avg(node_hwmon_temp_celsius{instance=\"' + name + '\"})'
dd['panels'][1]['gridPos']['y'] += num * 9

dd['panels'][2]['targets'][0]['expr'] = 'avg(irate(node_cpu_seconds_total{instance=\"' + name + '\",mode=\"system\"}[1m]))'
dd['panels'][2]['targets'][1]['expr'] = 'avg(irate(node_cpu_seconds_total{instance=\"' + name + '\",mode=\"user\"}[1m]))'
dd['panels'][2]['targets'][2]['expr'] = 'avg(irate(node_cpu_seconds_total{instance=\"' + name + '\",mode=\"idle\"}[1m]))'
dd['panels'][2]['targets'][3]['expr'] = 'avg(irate(node_cpu_seconds_total{instance=\"' + name + '\",mode=\"iowait\"}[1m]))'
dd['panels'][2]['gridPos']['y'] += num * 9

dd['panels'][3]['targets'][0]['expr'] = '100 - (avg(irate(node_cpu_seconds_total{instance=\"' + name + '\",mode=\"idle\"}[1m])) * 100)'
dd['panels'][3]['gridPos']['y'] += num * 9

dd['panels'][4]['targets'][0]['expr'] = 'irate(node_network_receive_bytes_total{instance=\"'+ name + '\"}[5m]) * 8'
dd['panels'][4]['gridPos']['y'] += num * 9

dd['panels'][5]['targets'][0]['expr'] = 'irate(node_network_transmit_bytes_total{instance=\"' + name +'\"}[5m])*8'
dd['panels'][5]['gridPos']['y'] += num * 9

dd['panels'][6]['targets'][0]['expr'] = 'node_procs_running{instance=\"' + name + '\"}'
dd['panels'][6]['gridPos']['y'] += num * 9

dd['panels'][7]['targets'][0]['expr'] = '((node_memory_MemTotal_bytes{instance=\"' + name + '\"} - node_memory_MemFree_bytes{instance=\"' + name + '\"} - node_memory_Buffers_bytes{instance=\"' + name + '\"} - node_memory_Cached_bytes{instance=\"' + name + '\"}) / (node_memory_MemTotal_bytes{instance=\"' + name + '\"} )) * 100'
dd['panels'][7]['gridPos']['y'] += num * 9

file_path_out = "./grafana.json"
with open(file_path_out, 'r') as file:
    data = json.load(file)
file.close()

data['panels'] +=dd['panels']
with open(file_path_out, "w", encoding="utf-8") as f_new:
    json.dump(data, f_new, ensure_ascii=False, indent=4)
    f_new.close()
```



## 碰到的问题及解决方法：

1.端口被占用：

```shell
sudo lsof -i:3000 #查询占用3000端口的进程
sudo kill -9 2068 
```

2.如何按需求配置**Grafana**看板：

直接修改存储看板格式的*.json*文件。

3.有些主机无法通过**9100**端口访问，暂时无法解决，可能是防火墙太强大的原因

4.**prometheus**.*service*启动失败

产生原因：下载的版本为**prometheus-3.0.0-beta.0.linux-amd64**，版本过高，与**wsl2**下的**Ubuntu**版本不兼容

解决方案：降低**prometheus**的版本为**prometheus-2.55.0-rc.0.linux-amd64**

5.自己写的**shell**脚本无法运行

产生原因：*.sh*脚本文件缺少执行权限

解决方案：终端执行指令

```shell
chmod +x .sh
```

6.**shell**脚本无法开机自启，即无法自动化部署

解决方案：生成*.service*后缀文件，并执行指令将服务配置为开机自启  
**sudo systemctl enable my_prometheus_update.service**

```service
[Unit]
Description=My Service
After=network.target

[Service]
ExecStart=/home/cjj/hpc/experiment_9_task6/make_yml.sh
WorkingDirectory=/home/cjj/hpc/experiment_9_task6/

[Install]
WantedBy=default.target
```

7.*.json*文件迁移过程中会出现*uid*错误的情况

解决方案：点击更改*uid*选项，填入已有的数据源的*uid*。
