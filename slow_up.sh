# スクリプト例: slow_upload.sh
while true; do
  echo -n "x"  # 1バイト送信
  sleep 1      # 1秒待機
done | curl -i -X POST --data-binary @- http://localhost:8081/docs/upload/