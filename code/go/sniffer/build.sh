PWD=`pwd`
GIT_PATH='github.com/zzqcn'
SRC_PATH=${GOPATH}/src/${GIT_PATH}
mkdir -p ${SRC_PATH}
ln -sf ${PWD} ${SRC_PATH}
go build -o ${GOPATH}/bin/sniffer ${GIT_PATH}/sniffer
cd ${PWD}

