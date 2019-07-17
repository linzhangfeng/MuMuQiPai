kill -9 $(lsof -i:9001 |awk '{print $2}' | tail -n 2)  
kill -9 $(lsof -i:9000 |awk '{print $2}' | tail -n 2)  
kill -9 $(lsof -i:9003 |awk '{print $2}' | tail -n 2)  
kill -9 $(lsof -i:10000 |awk '{print $2}' | tail -n 2)  