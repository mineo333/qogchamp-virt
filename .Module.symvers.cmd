cmd_/home/mineo333/virt/Module.symvers := sed 's/\.ko$$/\.o/' /home/mineo333/virt/modules.order | scripts/mod/modpost -m -a  -o /home/mineo333/virt/Module.symvers -e -i Module.symvers   -T -
