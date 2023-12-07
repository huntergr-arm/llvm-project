#include "AArch64Subtarget.h"
#include "AArch64TargetMachine.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include "gtest/gtest.h"
#include <initializer_list>
#include <memory>

using namespace llvm;

namespace {

struct AddrMode : public TargetLowering::AddrMode {
  constexpr AddrMode(GlobalValue *GV, int64_t Offs, bool ScalableOffs,
                     bool HasBase, int64_t S) {
    BaseGV = GV;
    BaseOffs = AddressOffset::get(Offs, ScalableOffs);
    HasBaseReg = HasBase;
    Scale = S;
  }
};
struct TestCase {
  AddrMode AM;
  unsigned TypeBits;
  bool Result;
};

const std::initializer_list<TestCase> Tests = {
    // {BaseGV, BaseOffs, ScalableOffs, HasBaseReg, Scale}, Bits, Result
    {{reinterpret_cast<GlobalValue *>(-1), 0, false, false, 0}, 64, false},
    {{nullptr, 8, false, true, 1}, 64, false},
    {{nullptr, 0, false, false, 2}, 64, true},
    {{nullptr, 0, false, false, 1}, 64, true},
    {{nullptr, 4, false, false, 0}, 64, false},

    {{nullptr, 0, false, true, 1}, 64, true},
    {{nullptr, 0, false, true, 1}, 32, true},
    {{nullptr, 0, false, true, 1}, 16, true},
    {{nullptr, 0, false, true, 1}, 8, true},

    {{nullptr, 0, false, true, 2}, 64, false},
    {{nullptr, 0, false, true, 2}, 32, false},
    {{nullptr, 0, false, true, 2}, 16, true},
    {{nullptr, 0, false, true, 2}, 8, false},
    {{nullptr, 0, false, true, 4}, 64, false},
    {{nullptr, 0, false, true, 4}, 32, true},
    {{nullptr, 0, false, true, 4}, 16, false},
    {{nullptr, 0, false, true, 4}, 8, false},

    {{nullptr, 0, false, true, 8}, 64, true},
    {{nullptr, 0, false, true, 8}, 32, false},
    {{nullptr, 0, false, true, 8}, 16, false},
    {{nullptr, 0, false, true, 8}, 8, false},

    {{nullptr, 0, false, true, 16}, 64, false},
    {{nullptr, 0, false, true, 16}, 32, false},
    {{nullptr, 0, false, true, 16}, 16, false},
    {{nullptr, 0, false, true, 16}, 8, false},

    {{nullptr, -257, false, true, 0}, 64, false},
    {{nullptr, -256, false, true, 0}, 64, true},
    {{nullptr, -255, false, true, 0}, 64, true},
    {{nullptr, -1, false, true, 0}, 64, true},
    {{nullptr, 0, false, true, 0}, 64, true},
    {{nullptr, 1, false, true, 0}, 64, true},
    {{nullptr, 254, false, true, 0}, 64, true},
    {{nullptr, 255, false, true, 0}, 64, true},
    {{nullptr, 256, false, true, 0}, 64, true},
    {{nullptr, 257, false, true, 0}, 64, false},
    {{nullptr, 258, false, true, 0}, 64, false},
    {{nullptr, 259, false, true, 0}, 64, false},
    {{nullptr, 260, false, true, 0}, 64, false},
    {{nullptr, 261, false, true, 0}, 64, false},
    {{nullptr, 262, false, true, 0}, 64, false},
    {{nullptr, 263, false, true, 0}, 64, false},
    {{nullptr, 264, false, true, 0}, 64, true},

    {{nullptr, 4096 * 8 - 8, false, true, 0}, 64, true},
    {{nullptr, 4096 * 8 - 7, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 6, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 5, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 4, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 3, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 2, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 - 1, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 1, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 2, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 3, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 4, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 5, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 6, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 7, false, true, 0}, 64, false},
    {{nullptr, 4096 * 8 + 8, false, true, 0}, 64, false},

    {{nullptr, -257, false, true, 0}, 32, false},
    {{nullptr, -256, false, true, 0}, 32, true},
    {{nullptr, -255, false, true, 0}, 32, true},
    {{nullptr, -1, false, true, 0}, 32, true},
    {{nullptr, 0, false, true, 0}, 32, true},
    {{nullptr, 1, false, true, 0}, 32, true},
    {{nullptr, 254, false, true, 0}, 32, true},
    {{nullptr, 255, false, true, 0}, 32, true},
    {{nullptr, 256, false, true, 0}, 32, true},
    {{nullptr, 257, false, true, 0}, 32, false},
    {{nullptr, 258, false, true, 0}, 32, false},
    {{nullptr, 259, false, true, 0}, 32, false},
    {{nullptr, 260, false, true, 0}, 32, true},

    {{nullptr, 4096 * 4 - 4, false, true, 0}, 32, true},
    {{nullptr, 4096 * 4 - 3, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 - 2, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 - 1, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 + 1, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 + 2, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 + 3, false, true, 0}, 32, false},
    {{nullptr, 4096 * 4 + 4, false, true, 0}, 32, false},

    {{nullptr, -257, false, true, 0}, 16, false},
    {{nullptr, -256, false, true, 0}, 16, true},
    {{nullptr, -255, false, true, 0}, 16, true},
    {{nullptr, -1, false, true, 0}, 16, true},
    {{nullptr, 0, false, true, 0}, 16, true},
    {{nullptr, 1, false, true, 0}, 16, true},
    {{nullptr, 254, false, true, 0}, 16, true},
    {{nullptr, 255, false, true, 0}, 16, true},
    {{nullptr, 256, false, true, 0}, 16, true},
    {{nullptr, 257, false, true, 0}, 16, false},
    {{nullptr, 258, false, true, 0}, 16, true},

    {{nullptr, 4096 * 2 - 2, false, true, 0}, 16, true},
    {{nullptr, 4096 * 2 - 1, false, true, 0}, 16, false},
    {{nullptr, 4096 * 2, false, true, 0}, 16, false},
    {{nullptr, 4096 * 2 + 1, false, true, 0}, 16, false},
    {{nullptr, 4096 * 2 + 2, false, true, 0}, 16, false},

    {{nullptr, -257, false, true, 0}, 8, false},
    {{nullptr, -256, false, true, 0}, 8, true},
    {{nullptr, -255, false, true, 0}, 8, true},
    {{nullptr, -1, false, true, 0}, 8, true},
    {{nullptr, 0, false, true, 0}, 8, true},
    {{nullptr, 1, false, true, 0}, 8, true},
    {{nullptr, 254, false, true, 0}, 8, true},
    {{nullptr, 255, false, true, 0}, 8, true},
    {{nullptr, 256, false, true, 0}, 8, true},
    {{nullptr, 257, false, true, 0}, 8, true},

    {{nullptr, 4096 - 2, false, true, 0}, 8, true},
    {{nullptr, 4096 - 1, false, true, 0}, 8, true},
    {{nullptr, 4096, false, true, 0}, 8, false},
    {{nullptr, 4096 + 1, false, true, 0}, 8, false},

};

struct VecTestCase {
  AddrMode AM;
  unsigned TypeBits;
  unsigned NumElts;
  bool Scalable;
  bool Result;
};

const std::initializer_list<VecTestCase> VecTests = {
    // Test immediate range -- [-8,7] vector's worth.
    // <vscale x 16 x i8>, increment by one vector
    {{nullptr, 16, true, true, 0}, 8, 16, true, true},
    // <vscale x 4 x i32>, increment by eight vectors
    {{nullptr, 128, true, true, 0}, 32, 4, true, false},
    // <vscale x 8 x i16>, increment by seven vectors
    {{nullptr, 112, true, true, 0}, 16, 8, true, true},
    // <vscale x 2 x i64>, decrement by eight vectors
    {{nullptr, -128, true, true, 0}, 64, 2, true, true},
    // <vscale x 16 x i8>, decrement by nine vectors
    {{nullptr, -144, true, true, 0}, 8, 16, true, false},

    // Test invalid types or offsets
    // <vscale x 5 x i32>, increment by one vector
    {{nullptr, 16, true, true, 0}, 32, 5, true, false},
    // <vscale x 8 x i16>, increment by half a vector
    {{nullptr, 8, true, true, 0}, 16, 8, true, false},
    // Non-scalable vector <2 x i64>
    {{nullptr, 24, true, true, 0}, 64, 2, false, false},
};

} // namespace

TEST(AddressingModes, AddressingModes) {
  LLVMInitializeAArch64TargetInfo();
  LLVMInitializeAArch64Target();
  LLVMInitializeAArch64TargetMC();

  std::string Error;
  auto TT = Triple::normalize("aarch64");
  const Target *T = TargetRegistry::lookupTarget(TT, Error);

  std::unique_ptr<TargetMachine> TM(
      T->createTargetMachine(TT, "generic", "", TargetOptions(), std::nullopt,
                             std::nullopt, CodeGenOptLevel::Default));
  AArch64Subtarget ST(TM->getTargetTriple(), TM->getTargetCPU(),
                      TM->getTargetCPU(), TM->getTargetFeatureString(), *TM,
                      true);

  auto *TLI = ST.getTargetLowering();
  DataLayout DL("e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128");
  LLVMContext Ctx;

  for (const auto &Test : Tests) {
    Type *Typ = Type::getIntNTy(Ctx, Test.TypeBits);
    ASSERT_EQ(TLI->isLegalAddressingMode(DL, Test.AM, Typ, 0), Test.Result);
  }

  for (const auto &VecTest : VecTests) {
    Type *Ty = VectorType::get(Type::getIntNTy(Ctx, VecTest.TypeBits),
                               ElementCount::get(VecTest.NumElts,
                                                 VecTest.Scalable));
    ASSERT_EQ(TLI->isLegalAddressingMode(DL, VecTest.AM, Ty, 0),
              VecTest.Result);
  }
}
