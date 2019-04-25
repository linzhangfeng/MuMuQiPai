#./build.sh dir tianmenmj 1.0.0 tianmen
dir=$1
project=$2
ver=$3
gameres=$4

mkdir output
rm -rf output/

mkdir -p output/${project}/scripts/
cp -rf ./${dir}/scripts/ output/${project}/scripts/

if [ "$gameres" == "all" ]  ;then
mkdir -p output/${project}/res/
cp -rf ./${dir}/res/ output/${project}/res/
fi

if [ "$gameres" != "all" ]  ;then
OLD_IFS="$IFS"
IFS="," 
arr=($gameres) 
IFS="$OLD_IFS" 
for s in ${arr[@]} 
do 
    mkdir -p output/${project}/res/${s}/
	cp -rf ./${dir}/res/${s}/ output/${project}/res/${s}/
done
fi


find output/${project} -name "*.svn" | xargs rm -rf
#cocos jscompile -s output/${project}/scripts/ -d output/${project}/scripts
#find output/${project} -name "*.js" | xargs rm -rf

read -p "即将开始打包，是否继续?"  val

node version_generator.js -s output -d output/${project}/version/ -sEx ${project}/ -vEx ${project}/version/  -v ${ver}
cd output
zip -r ${project}_${ver}.zip ${project}
cd ../