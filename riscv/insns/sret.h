require_privilege(PRV_S);
switch (STATE.prv)
{
  case PRV_S: set_pc(p->get_state()->sepc); break;
  case PRV_M: set_pc(p->get_state()->mepc); break;
  default: abort();
}

reg_t s = STATE.mstatus;
reg_t pie = get_field(s, MSTATUS_UPIE << STATE.prv);
reg_t prev_prv = get_field(s, STATE.prv == PRV_S ? MSTATUS_SPP : MSTATUS_MPP);
s = set_field(s, MSTATUS_UIE << prev_prv, pie); // [[prv]PP]IE = [prv]PIE
s = set_field(s, MSTATUS_UPIE << STATE.prv, 0); // [prv]PIE <- 0
p->set_privilege(prev_prv); // prv <- [prv]PP
s = set_field(s, STATE.prv == PRV_S ? MSTATUS_SPP : MSTATUS_MPP, PRV_U); // [prv]PP = U
p->set_csr(CSR_MSTATUS, s);
