//
// Copyright 2020 Ettus Research, A National Instruments Company
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// Module: ChdrIfaceBfm_all_tb
//
// Description:
//
//   This is the top-level testbench that instantiates multiple instances of
//   the ChdrIfaceBfm_tb testbench to test different parameters.
//

module ChdrIfaceBfm_all_tb();

  `include "test_exec.svh"

  //---------------------------------------------------------------------------
  // Test Definitions
  //---------------------------------------------------------------------------

  typedef struct {
    int CHDR_W;
    int ITEM_W;
  } test_config_t;

  localparam NUM_TESTS = 8;

  localparam test_config_t tests[NUM_TESTS] = '{
    '{ CHDR_W:  64, ITEM_W:  8 },
    '{ CHDR_W:  64, ITEM_W: 16 },
    '{ CHDR_W:  64, ITEM_W: 32 },
    '{ CHDR_W:  64, ITEM_W: 64 },
    '{ CHDR_W: 128, ITEM_W:  8 },
    '{ CHDR_W: 128, ITEM_W: 16 },
    '{ CHDR_W: 128, ITEM_W: 32 },
    '{ CHDR_W: 128, ITEM_W: 64 }
  };


  //---------------------------------------------------------------------------
  // DUT Instances
  //---------------------------------------------------------------------------

  genvar i;
  for (i = 0; i < NUM_TESTS; i++) begin : gen_test_config
    ChdrIfaceBfm_tb #(
      .CHDR_W (tests[i].CHDR_W),
      .ITEM_W (tests[i].ITEM_W)
    ) ChdrIfaceBfm_tb_i ();
  end : gen_test_config

endmodule : ChdrIfaceBfm_all_tb
