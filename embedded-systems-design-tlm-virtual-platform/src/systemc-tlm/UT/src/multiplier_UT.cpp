#include "multiplier_UT.h"

multiplier_UT::multiplier_UT(sc_module_name name_)
    : sc_module(name_), target_socket("target_socket"),
      pending_transaction(NULL) {

    target_socket(*this);
}

void multiplier_UT::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {
    ioDataStruct = *((iostruct *)trans.get_data_ptr());

    if (trans.is_write()) {
        multiplier_function();
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        ioDataStruct.result = tmp_result;
        *((iostruct *)trans.get_data_ptr()) = ioDataStruct;
    }
    else if (trans.is_read()) {
        ioDataStruct.result = tmp_result;
        *((iostruct *)trans.get_data_ptr()) = ioDataStruct;
    }
}

bool multiplier_UT::get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                                       tlm::tlm_dmi &dmi_data) {
    return false;
}

tlm::tlm_sync_enum
multiplier_UT::nb_transport_fw(tlm::tlm_generic_payload &trans,
                               tlm::tlm_phase &phase, sc_time &t) {
    return tlm::TLM_COMPLETED;
}

unsigned int multiplier_UT::transport_dbg(tlm::tlm_generic_payload &trans) {
    return 0;
}

void multiplier_UT::multiplier_function() {
    ieee754_single_precision tmp_op1, tmp_op2, mul;

    tmp_op1.uint = ioDataStruct.datain_op1;
    tmp_op2.uint = ioDataStruct.datain_op2;
    mul.floating_point = tmp_op1.floating_point * tmp_op2.floating_point;

    tmp_result = mul.uint;
}

// Initialization:
void multiplier_UT::end_of_elaboration() {}

void multiplier_UT::reset() {}
