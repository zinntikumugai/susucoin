// Copyright (c) 2015-2017 The Bitcoin Core developers
// Copyright (c) 2018 The Susucoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chain.h>
#include <chainparams.h>
#include <pow.h>
#include <random.h>
#include <util.h>
#include <test/test_bitcoin.h>

#include <boost/test/unit_test.hpp>

static CBlockIndex GetBlockIndex(CBlockIndex *pindexPrev, int64_t nTimeInterval,
                                 uint32_t nBits) {
    CBlockIndex block;
    block.pprev = pindexPrev;
    block.nHeight = pindexPrev->nHeight + 1;
    block.nTime = pindexPrev->nTime + nTimeInterval;
    block.nBits = nBits;

    block.nChainWork = pindexPrev->nChainWork + GetBlockProof(block);
    return block;
}

BOOST_FIXTURE_TEST_SUITE(pow_tests, BasicTestingSetup)

/* Test calculation of next difficulty target with no constraints applying */
BOOST_AUTO_TEST_CASE(get_next_work)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    int64_t nLastRetargetTime = 1261130161; // Block #30240
    CBlockIndex pindexLast;
    pindexLast.nHeight = 32255;
    pindexLast.nTime = 1262152739;  // Block #32255
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(DigishieldGetNextWorkRequired(&pindexLast, nullptr, chainParams->GetConsensus()), 0x1f0fffffU);
}

/* Test the constraint on the upper bound for next work */
BOOST_AUTO_TEST_CASE(get_next_work_pow_limit)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    int64_t nLastRetargetTime = 1231006505; // Block #0
    CBlockIndex pindexLast;
    pindexLast.nHeight = 2015;
    pindexLast.nTime = 1233061996;  // Block #2015
    pindexLast.nBits = 0x1d00ffff;
    BOOST_CHECK_EQUAL(DigishieldGetNextWorkRequired(&pindexLast, nullptr, chainParams->GetConsensus()), 0x1f0fffffU);
}

/* Test the constraint on the lower bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_lower_limit_actual)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    int64_t nLastRetargetTime = 1279008237; // Block #66528
    CBlockIndex pindexLast;
    pindexLast.nHeight = 68543;
    pindexLast.nTime = 1279297671;  // Block #68543
    pindexLast.nBits = 0x1c05a3f4;
    BOOST_CHECK_EQUAL(DigishieldGetNextWorkRequired(&pindexLast, nullptr, chainParams->GetConsensus()), 0x1f0fffffU);
}

/* Test the constraint on the upper bound for actual time taken */
BOOST_AUTO_TEST_CASE(get_next_work_upper_limit_actual)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    int64_t nLastRetargetTime = 1263163443; // NOTE: Not an actual block time
    CBlockIndex pindexLast;
    pindexLast.nHeight = 46367;
    pindexLast.nTime = 1269211443;  // Block #46367
    pindexLast.nBits = 0x1c387f6f;
    BOOST_CHECK_EQUAL(DigishieldGetNextWorkRequired(&pindexLast, nullptr, chainParams->GetConsensus()), 0x1f0fffffU);
}

BOOST_AUTO_TEST_CASE(GetBlockProofEquivalentTime_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    std::vector<CBlockIndex> blocks(10000);
    for (int i = 0; i < 10000; i++) {
        blocks[i].pprev = i ? &blocks[i - 1] : nullptr;
        blocks[i].nHeight = i;
        blocks[i].nTime = 1269211443 + i * chainParams->GetConsensus().nPowTargetSpacing;
        blocks[i].nBits = 0x207fffff; /* target 0x7fffff000... */
        blocks[i].nChainWork = i ? blocks[i - 1].nChainWork + GetBlockProof(blocks[i - 1]) : arith_uint256(0);
    }

    for (int j = 0; j < 1000; j++) {
        CBlockIndex *p1 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p2 = &blocks[InsecureRandRange(10000)];
        CBlockIndex *p3 = &blocks[InsecureRandRange(10000)];

        int64_t tdiff = GetBlockProofEquivalentTime(*p1, *p2, *p3, chainParams->GetConsensus());
        BOOST_CHECK_EQUAL(tdiff, p1->GetBlockTime() - p2->GetBlockTime());
    }
}

BOOST_AUTO_TEST_CASE(lwma_difficulty_test) {
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);

    std::vector<CBlockIndex> blocks(3000);

    const arith_uint256 powLimit = UintToArith256(chainParams->GetConsensus().powLimit);
    uint32_t powLimitBits = powLimit.GetCompact();
    arith_uint256 currentPow = powLimit >> 4;
    uint32_t initialBits = currentPow.GetCompact();

    // Genesis block.
    blocks[0] = CBlockIndex();
    blocks[0].nHeight = 0;
    blocks[0].nTime = 1269211443;
    blocks[0].nBits = initialBits;

    blocks[0].nChainWork = GetBlockProof(blocks[0]);

    // Block counter.
    size_t i;

    // Create the first window for lwma, with blocks every 10 minutes.
    for (i = 1; i < 62; i++) {
        blocks[i] = GetBlockIndex(&blocks[i - 1], 600, initialBits);
    }

    uint32_t nBits =
        LwmaCalculateNextWorkRequired(&blocks[61], chainParams->GetConsensus());

    // For the first window, with 10 minutes between blocks, the difficulty should be low.
    BOOST_CHECK_EQUAL( nBits, 0x1f04fffaU );
    // == 0004fffa00000000000000000000000000000000000000000000000000000000 == 8.83407e+72

    // Add one block far in the future.
    blocks[i] = GetBlockIndex(&blocks[i - 1], 6000, nBits);
    // The difficulty is now a somewhat lower.
    nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
    BOOST_CHECK_EQUAL( nBits, 0x1f055e43U );
    // == 00055e4300000000000000000000000000000000000000000000000000000000 == 9.48481e+72

    // Add another block with a normal timestamp.
    blocks[i] = GetBlockIndex(&blocks[i - 1], 2 * 600 - 6000, nBits);
    nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
    // The difficulty is now just a little bit lower, again.
    BOOST_CHECK_EQUAL( nBits, 0x1f056f64U );
    // == 00056f6400000000000000000000000000000000000000000000000000000000 == 9.60303e+72

    // And another block with a regular timestamp.
    blocks[i] = GetBlockIndex(&blocks[i - 1], 600, nBits);
    // The difficulty has lowered yet again, by a fraction.
    nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
    BOOST_CHECK_EQUAL( nBits, 0x1f05cad8U );
    // == 0005cad800000000000000000000000000000000000000000000000000000000 == 1.02342e+73

    // Simulate a hash attack, add a window with very low increase.
    for ( int j = 0; j < 10; j++ ) {
        // first, add one block with one second interval
        blocks[i] = GetBlockIndex(&blocks[i - 1], 1, nBits);
        nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
	// then add five more with zero second interval
        for ( int k = 0; k < 5; k++ ) {
            blocks[i] = GetBlockIndex(&blocks[i - 1], 0, nBits);
            nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
        }
        // and do that ten times.  That gives us 60 block window with very high frequency
        // of blocks.
    }
    // The difficulty is now significantly higher.
    BOOST_CHECK_EQUAL( nBits, 0x1e677232U );
    // == 0000677232000000000000000000000000000000000000000000000000000000 == 7.13959e+71

    // Add one more block with a significant delay.
    blocks[i] = GetBlockIndex(&blocks[i - 1], 4 * 3600, nBits);
    // The difficulty has lowered significantly.
    nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
    BOOST_CHECK_EQUAL( nBits, 0x1f00c838U );
    // == 0000c83800000000000000000000000000000000000000000000000000000000 == 1.38186e+72

    // One more block with little less delay.
    blocks[i] = GetBlockIndex(&blocks[i - 1], 2 * 3600, nBits);
    // The difficulty has lowered again.
    nBits = LwmaCalculateNextWorkRequired(&blocks[i++], chainParams->GetConsensus());
    BOOST_CHECK_EQUAL( nBits, 0x1f01832fU );
    // == 0001832f00000000000000000000000000000000000000000000000000000000 == 2.67224e+72
}

BOOST_AUTO_TEST_SUITE_END()
