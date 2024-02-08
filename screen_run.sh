g++ -o main_f main_f.cpp
while read rows
do
screen_name="Ring"$RANDOM
screen -dmS $screen_name
screen -r $screen_name -p 0 -X stuff "$rows`echo -ne '\015'`"
screen -r $screen_name -p 0 -X stuff "exit`echo -ne '\015'`"
done < config.sh