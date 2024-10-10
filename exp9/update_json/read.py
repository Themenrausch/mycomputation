import json
import sys

# name = '10.249.12.89:9100'

name = sys.argv[1]

# 假设有一个名为example.json的文件
file_path_in = "./grafana_proto.json"
 
# 使用json.load()读取文件内容到一个字典
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