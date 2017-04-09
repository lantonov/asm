
MAX_RESETCNT = 200000
MIN_RESETCNT = 40
SPAMFILTER_DELAY = 100
CURRMOVE_MIN_TIME = 3000

AVG_MOVES = 96 
MAX_MOVES = 224
MAX_THREADS = 256
MAX_NUMANODES = 32
MAX_LINUXCPUS = 512
MAX_HASH_LOG2MB = 16
THREAD_STACK_SIZE = 1048576
PAWN_HASH_ENTRY_COUNT = 16384
MATERIAL_HASH_ENTRY_COUNT = 8192

RANK_8 = 7
RANK_7 = 6
RANK_6 = 5
RANK_5 = 4
RANK_4 = 3
RANK_3 = 2
RANK_2 = 1
RANK_1 = 0

FILE_H = 7
FILE_G = 6
FILE_F = 5
FILE_E = 4
FILE_D = 3
FILE_C = 2
FILE_B = 1
FILE_A = 0

SQ_A1 = (0+8*0)
SQ_B1 = (1+8*0)
SQ_C1 = (2+8*0)
SQ_D1 = (3+8*0)
SQ_E1 = (4+8*0)
SQ_F1 = (5+8*0)
SQ_G1 = (6+8*0)
SQ_H1 = (7+8*0)

SQ_A2 = (0+8*1)
SQ_B2 = (1+8*1)
SQ_C2 = (2+8*1)
SQ_D2 = (3+8*1)
SQ_E2 = (4+8*1)
SQ_F2 = (5+8*1)
SQ_G2 = (6+8*1)
SQ_H2 = (7+8*1)

SQ_A3 = (0+8*2)
SQ_B3 = (1+8*2)
SQ_C3 = (2+8*2)
SQ_D3 = (3+8*2)
SQ_E3 = (4+8*2)
SQ_F3 = (5+8*2)
SQ_G3 = (6+8*2)
SQ_H3 = (7+8*2)

SQ_A4 = (0+8*3)
SQ_B4 = (1+8*3)
SQ_C4 = (2+8*3)
SQ_D4 = (3+8*3)
SQ_E4 = (4+8*3)
SQ_F4 = (5+8*3)
SQ_G4 = (6+8*3)
SQ_H4 = (7+8*3)

SQ_A5 = (0+8*4)
SQ_B5 = (1+8*4)
SQ_C5 = (2+8*4)
SQ_D5 = (3+8*4)
SQ_E5 = (4+8*4)
SQ_F5 = (5+8*4)
SQ_G5 = (6+8*4)
SQ_H5 = (7+8*4)

SQ_A6 = (0+8*5)
SQ_B6 = (1+8*5)
SQ_C6 = (2+8*5)
SQ_D6 = (3+8*5)
SQ_E6 = (4+8*5)
SQ_F6 = (5+8*5)
SQ_G6 = (6+8*5)
SQ_H6 = (7+8*5)

SQ_A7 = (0+8*6)
SQ_B7 = (1+8*6)
SQ_C7 = (2+8*6)
SQ_D7 = (3+8*6)
SQ_E7 = (4+8*6)
SQ_F7 = (5+8*6)
SQ_G7 = (6+8*6)
SQ_H7 = (7+8*6)

SQ_A8 = (0+8*7)
SQ_B8 = (1+8*7)
SQ_C8 = (2+8*7)
SQ_D8 = (3+8*7)
SQ_E8 = (4+8*7)
SQ_F8 = (5+8*7)
SQ_G8 = (6+8*7)
SQ_H8 = (7+8*7)

// some bitboards
DarkSquares  = 0xAA55AA55AA55AA55
LightSquares = 0x55AA55AA55AA55AA
FileABB   = 0x0101010101010101
FileBBB   = 0x0202020202020202
FileCBB   = 0x0404040404040404
FileDBB   = 0x0808080808080808
FileEBB   = 0x1010101010101010
FileFBB   = 0x2020202020202020
FileGBB   = 0x4040404040404040
FileHBB   = 0x8080808080808080
Rank8BB   = 0xFF00000000000000
Rank7BB   = 0x00FF000000000000
Rank6BB   = 0x0000FF0000000000
Rank5BB   = 0x000000FF00000000
Rank4BB   = 0x00000000FF000000
Rank3BB   = 0x0000000000FF0000
Rank2BB   = 0x000000000000FF00
Rank1BB   = 0x00000000000000FF

// move types
MOVE_TYPE_NORMAL = 0
MOVE_TYPE_PROM   = 4
MOVE_TYPE_EPCAP  = 8
MOVE_TYPE_CASTLE = 12
// special moves (should be negative as 32bit quantities)
MOVE_NONE    = 0
MOVE_NULL    = 65 + 0x0FFFFF000

White  = 0
Black  = 1
Pawn   = 2
Knight = 3
Bishop = 4
Rook   = 5
Queen  = 6
King   = 7

CAPTURES     = 0
QUIETS       = 1
QUIET_CHECKS = 2
EVASIONS     = 3
NON_EVASIONS = 4
LEGAL        = 5

// piece values
PawnValueMg   = 188
KnightValueMg = 753
BishopValueMg = 814
RookValueMg   = 1285
QueenValueMg  = 2513

PawnValueEg   = 248
KnightValueEg = 832
BishopValueEg = 890
RookValueEg   = 1371
QueenValueEg  = 2648

MidgameLimit = 15258
EndgameLimit = 3915

// values for evaluation
Eval_Tempo = 20

// values from stats tables
HistoryStats_Max = 268435456

// values for evaluation
VALUE_ZERO	 = 0
VALUE_DRAW	 = 0
VALUE_KNOWN_WIN  = 10000
VALUE_MATE	 = 32000
VALUE_INFINITE   = 32001
VALUE_NONE	 = 32002
VALUE_MATE_IN_MAX_PLY	=  VALUE_MATE - 2 * MAX_PLY
VALUE_MATED_IN_MAX_PLY = -VALUE_MATE_IN_MAX_PLY

PHASE_MIDGAME	 = 128

SCALE_FACTOR_DRAW    = 0
SCALE_FACTOR_ONEPAWN = 48
SCALE_FACTOR_NORMAL  = 64
SCALE_FACTOR_MAX     = 128
SCALE_FACTOR_NONE    = 255


// depths for search
ONE_PLY = 1
MAX_PLY = 128
DEPTH_QS_CHECKS     =	0
DEPTH_QS_NO_CHECKS  = -1
DEPTH_QS_RECAPTURES = -5
DEPTH_NONE	     = -6

// definitions for move gen macros
CAPTURES     = 0
QUIETS       = 1
QUIET_CHECKS = 2
EVASIONS     = 3
NON_EVASIONS = 4
LEGAL	      = 5

DELTA_N =  8
DELTA_E =  1
DELTA_S = -8
DELTA_W = -1

DELTA_NN = 16
DELTA_NE = 9
DELTA_SE = -7
DELTA_SS = -16
DELTA_SW = -9
DELTA_NW = 7

// bounds           don't change
BOUND_NONE  = 0
BOUND_UPPER = 1
BOUND_LOWER = 2
BOUND_EXACT = 3

// endgame eval fxn indices  see Endgames_Int.asm for details
EndgameEval_KPK_index	= 1  // KP vs K
EndgameEval_KNNK_index	= 2  // KNN vs K
EndgameEval_KBNK_index	= 3  // KBN vs K
EndgameEval_KRKP_index	= 4  // KR vs KP
EndgameEval_KRKB_index	= 5  // KR vs KB
EndgameEval_KRKN_index	= 6  // KR vs KN
EndgameEval_KQKP_index	= 7  // KQ vs KP
EndgameEval_KQKR_index	= 8  // KQ vs KR

ENDGAME_EVAL_MAP_SIZE = 8  // this should be number of functions added to the eval map

EndgameEval_KXK_index	= 10 // Generic mate lone king eval

ENDGAME_EVAL_MAX_INDEX = 16

// endgame scale fxn indices  see Endgames_Int.asm for details
EndgameScale_KNPK_index    = 1  // KNP vs K
EndgameScale_KNPKB_index   = 2  // KNP vs KB
EndgameScale_KRPKR_index   = 3  // KRP vs KR
EndgameScale_KRPKB_index   = 4  // KRP vs KB
EndgameScale_KBPKB_index   = 5  // KBP vs KB
EndgameScale_KBPKN_index   = 6  // KBP vs KN
EndgameScale_KBPPKB_index  = 7  // KBPP vs KB
EndgameScale_KRPPKRP_index = 8  // KRPP vs KRP

ENDGAME_SCALE_MAP_SIZE = 8  // this should be number of functions added to the eval map

EndgameScale_KBPsK_index   = 10 // KB and pawns vs K
EndgameScale_KQKRPs_index  = 11 // KQ vs KR and pawns
EndgameScale_KPsK_index    = 12 // K and pawns vs K
EndgameScale_KPKP_index    = 13 // KP vs KP

ENDGAME_SCALE_MAX_INDEX = 16



// hacky structs defs

sizeof.rb = 1
sizeof.rw = 2
sizeof.rd = 4
sizeof.rq = 8

MainHash.___ = 0
MainHash.table = 0 + MainHash.___
MainHash.mask = 1*sizeof.rq + MainHash.table
MainHash.lpSize = 1*sizeof.rq + MainHash.mask
MainHash.sizeMB = 1*sizeof.rq + MainHash.lpSize
MainHash.date = 1*sizeof.rd + MainHash.sizeMB
sizeof.MainHash = 1*sizeof.rb + 3*sizeof.rb + MainHash.date

MainHashEntry.___ = 0
MainHashEntry.genBound = 0 + MainHashEntry.___
MainHashEntry.depth = 1*sizeof.rb + MainHashEntry.genBound
MainHashEntry.move = 1*sizeof.rb + MainHashEntry.depth
MainHashEntry.eval = 1*sizeof.rw + MainHashEntry.move
MainHashEntry.value = 1*sizeof.rw + MainHashEntry.eval
sizeof.MainHashEntry = 1*sizeof.rw + MainHashEntry.value

MaterialEntry.___ = 0
MaterialEntry.key = 0 + MaterialEntry.___
MaterialEntry.scalingFunction = 1*sizeof.rq + MaterialEntry.key
MaterialEntry.evaluationFunction = 2*sizeof.rb + MaterialEntry.scalingFunction
MaterialEntry.gamePhase = 1*sizeof.rb + MaterialEntry.evaluationFunction
MaterialEntry.factor = 1*sizeof.rb + MaterialEntry.gamePhase
MaterialEntry.value = 2*sizeof.rb + MaterialEntry.factor
sizeof.MaterialEntry = 1*sizeof.rw + MaterialEntry.value

PawnEntry.___ = 0
PawnEntry.passedPawns = 0 + PawnEntry.___
PawnEntry.pawnAttacks = 2*sizeof.rq + PawnEntry.passedPawns
PawnEntry.pawnAttacksSpan = 2*sizeof.rq + PawnEntry.pawnAttacks
PawnEntry.key = 2*sizeof.rq + PawnEntry.pawnAttacksSpan
PawnEntry.kingSafety = 1*sizeof.rq + PawnEntry.key
PawnEntry.score = 2*sizeof.rd + PawnEntry.kingSafety
PawnEntry.kingSquares = 1*sizeof.rd + PawnEntry.score
PawnEntry.semiopenFiles = 2*sizeof.rb + PawnEntry.kingSquares
PawnEntry.pawnsOnSquares = 2*sizeof.rb + PawnEntry.semiopenFiles
PawnEntry.asymmetry = 4*sizeof.rb + PawnEntry.pawnsOnSquares
PawnEntry.openFiles = 1*sizeof.rb + PawnEntry.asymmetry
PawnEntry.castlingRights = 1*sizeof.rb + PawnEntry.openFiles
sizeof.PawnEntry = 1*sizeof.rb + 1*sizeof.rb + PawnEntry.castlingRights

HistoryStats.___ = 0
sizeof.HistoryStats = 0 + 2*64*64*sizeof.rd + HistoryStats.___

MoveStats.___ = 0
sizeof.MoveStats = 0 + 16*64*sizeof.rd + MoveStats.___

CounterMoveHistoryStats.___ = 0
sizeof.CounterMoveHistoryStats = 0 + 16*64*16*64*sizeof.rd + CounterMoveHistoryStats.___

EvalInfo.___ = 0
EvalInfo.attackedBy = 0 + EvalInfo.___
EvalInfo.attackedBy2 = 16*sizeof.rq + EvalInfo.attackedBy
EvalInfo.pinnedPieces = 2*sizeof.rq + EvalInfo.attackedBy2
EvalInfo.mobilityArea = 2*sizeof.rq + EvalInfo.pinnedPieces
EvalInfo.kingRing = 2*sizeof.rq + EvalInfo.mobilityArea
EvalInfo.kingAttackersCount = 2*sizeof.rq + EvalInfo.kingRing
EvalInfo.kingAttackersWeight = 2*sizeof.rd + EvalInfo.kingAttackersCount
EvalInfo.kingAdjacentZoneAttacksCount = 2*sizeof.rd + EvalInfo.kingAttackersWeight
EvalInfo.ksq = 2*sizeof.rd + EvalInfo.kingAdjacentZoneAttacksCount
EvalInfo.me = 2*sizeof.rd + EvalInfo.ksq
EvalInfo.pi = 1*sizeof.rq + EvalInfo.me
EvalInfo.score = 1*sizeof.rq + EvalInfo.pi
sizeof.EvalInfo = 1*sizeof.rd + 1*sizeof.rd + 2*sizeof.rd + EvalInfo.score

EndgameMapEntry.___ = 0
EndgameMapEntry.key = 0 + EndgameMapEntry.___
EndgameMapEntry.entri = 1*sizeof.rq + EndgameMapEntry.key
sizeof.EndgameMapEntry = 1*sizeof.rb + 7*sizeof.rb + EndgameMapEntry.entri

ExtMove.___ = 0
ExtMove.move = 0 + ExtMove.___
ExtMove.score = 1*sizeof.rd + ExtMove.move
sizeof.ExtMove = 1*sizeof.rd + ExtMove.score

RootMovesVec.___ = 0
RootMovesVec.table = 0 + RootMovesVec.___
RootMovesVec.ender = 1*sizeof.rq + RootMovesVec.table
sizeof.RootMovesVec = 1*sizeof.rq + RootMovesVec.ender

RootMove.___ = 0
RootMove.prevScore = 0 + RootMove.___
RootMove.score = 1*sizeof.rd + RootMove.prevScore
RootMove.pvSize = 1*sizeof.rd + RootMove.score
RootMove.pv = 1*sizeof.rd + 1*sizeof.rd + RootMove.pvSize
sizeof.RootMove = MAX_PLY*sizeof.rd + RootMove.pv

Pos.___ = 0
Pos.typeBB = 0 + Pos.___
Pos.board = 8*sizeof.rq + Pos.typeBB
Pos.pieceIdx = 64*sizeof.rb + Pos.board
Pos.pieceEnd = 64*sizeof.rb + Pos.pieceIdx
Pos.pieceList = 16*sizeof.rb + Pos.pieceEnd
Pos.sideToMove = 16*16*sizeof.rb + Pos.pieceList
Pos.gamePly = 1*sizeof.rd + 1*sizeof.rd + Pos.sideToMove
Pos.chess960 = 1*sizeof.rd + Pos.gamePly
Pos._copy_size = 1*sizeof.rd + Pos.chess960
Pos.state = 0*sizeof.rb + Pos._copy_size
Pos.stateTable = 1*sizeof.rq + Pos.state
Pos.stateEnd = 1*sizeof.rq + Pos.stateTable
Pos.counterMoveHistory = 1*sizeof.rq + Pos.stateEnd
Pos.history = 1*sizeof.rq + Pos.counterMoveHistory
Pos.counterMoves = 1*sizeof.rq + Pos.history
Pos.materialTable = 1*sizeof.rq + Pos.counterMoves
Pos.pawnTable = 1*sizeof.rq + Pos.materialTable
Pos.rootMovesVec = 1*sizeof.rq + Pos.pawnTable
Pos.moveList = 1*sizeof.RootMovesVec + Pos.rootMovesVec
sizeof.Pos = 1*sizeof.rq + Pos.moveList

.if Pos.typeBB
 .error "Pos.typeBB is not zero"
.endif

State.___ = 0
State.key = 0 + State.___
State.pawnKey = 1*sizeof.rq + State.key
State.materialKey = 1*sizeof.rq + State.pawnKey
State.psq = 1*sizeof.rq + State.materialKey
State.npMaterial = 2*sizeof.rw + State.psq
State.rule50 = 2*sizeof.rw + State.npMaterial
State.pliesFromNull = 1*sizeof.rw + State.rule50
State.epSquare = 1*sizeof.rw + State.pliesFromNull
State.castlingRights = 1*sizeof.rb + State.epSquare
State.capturedPiece = 1*sizeof.rb + State.castlingRights
State.ksq = 1*sizeof.rb + State.capturedPiece
State.checkersBB = 1*sizeof.rb + State.ksq
State.dcCandidates = 1*sizeof.rq + State.checkersBB
State.pinned = 1*sizeof.rq + State.dcCandidates
State.checkSq = 1*sizeof.rq + State.pinned
State.blockersForKing = 8*sizeof.rq + State.checkSq
State.pinnersForKing = 2*sizeof.rq + State.blockersForKing
State._stack_start = 2*sizeof.rq + State.pinnersForKing
State.pv = 0*sizeof.rb + State._stack_start
State.counterMoves = 1*sizeof.rq + State.pv
State.currentMove = 1*sizeof.rq + State.counterMoves
State.excludedMove = 1*sizeof.rd + State.currentMove
State.killers = 1*sizeof.rd + State.excludedMove
State.moveCount = 2*sizeof.rd + State.killers
State.staticEval = 1*sizeof.rd + State.moveCount
State.history = 1*sizeof.rd + State.staticEval
State.ply = 1*sizeof.rd + State.history
State.skipEarlyPruning = 1*sizeof.rb + State.ply
State._stack_end = 1*sizeof.rb + 2*sizeof.rb + State.skipEarlyPruning
State._movepick_start = 0*sizeof.rb + State._stack_end
State.cur               = 0*sizeof.rb + State._movepick_start
State.endMoves          = 1*sizeof.rq + State.cur
State.endBadCaptures    = 1*sizeof.rq + State.endMoves
State.stage             = 1*sizeof.rq + State.endBadCaptures
State.countermove       = 1*sizeof.rq + State.stage
State.givesCheck        = 1*sizeof.rd + State.countermove
State.ttMove            = 1*sizeof.rb + 3*sizeof.rb + State.givesCheck
State.depth             = 1*sizeof.rd + State.ttMove
State.threshold         = 1*sizeof.rd + State.depth
State.recaptureSquare   = 1*sizeof.rd + State.threshold
State._movepick_end     = 1*sizeof.rd + 2*sizeof.rd + State.recaptureSquare
sizeof.State            = 0*sizeof.rb + State._movepick_end

.if sizeof.State & 15
 .error "sizeof.State is bad"
.endif

Limits.___ = 0
Limits.nodes = 0 + Limits.___
Limits.startTime = 1*sizeof.rq + Limits.nodes
Limits.time = 1*sizeof.rq + Limits.startTime
Limits.incr = 2*sizeof.rd + Limits.time
Limits.movestogo = 2*sizeof.rd + Limits.incr
Limits.depth = 1*sizeof.rd + Limits.movestogo
Limits.movetime = 1*sizeof.rd + Limits.depth
Limits.mate = 1*sizeof.rd + Limits.movetime
Limits.multiPV = 1*sizeof.rd + Limits.mate
Limits.infinite = 1*sizeof.rd + 1*sizeof.rd + Limits.multiPV
Limits.ponder = 1*sizeof.rb + Limits.infinite
Limits.useTimeMgmt = 1*sizeof.rb + Limits.ponder
Limits.moveVecSize = 1*sizeof.rb + 1*sizeof.rb + Limits.useTimeMgmt
Limits.moveVec = 1*sizeof.rd + Limits.moveVecSize
sizeof.Limits = MAX_MOVES*sizeof.rw + Limits.moveVec

Options.___ = 0
Options.hash = 0 + Options.___
Options.threads = 1*sizeof.rd + Options.hash
Options.largePages = 1*sizeof.rd + Options.threads
Options.changed = 1*sizeof.rb + Options.largePages
Options.multiPV = 1*sizeof.rb + 2*sizeof.rb + Options.changed
Options.chess960 = 1*sizeof.rd + Options.multiPV
Options.minThinkTime = 1*sizeof.rd + Options.chess960
Options.slowMover = 1*sizeof.rd + Options.minThinkTime
Options.moveOverhead = 1*sizeof.rd + Options.slowMover
Options.contempt = 1*sizeof.rd + Options.moveOverhead
Options.ponder = 1*sizeof.rd + Options.contempt
Options.displayInfoMove = 1*sizeof.rb + Options.ponder
Options.syzygy50MoveRule = 1*sizeof.rb + 1*sizeof.rb + Options.displayInfoMove
Options.syzygyProbeDepth = 1*sizeof.rb + Options.syzygy50MoveRule
Options.syzygyProbeLimit = 1*sizeof.rd + Options.syzygyProbeDepth
sizeof.Options = 1*sizeof.rd + Options.syzygyProbeLimit

EasyMoveMng.___ = 0
EasyMoveMng.expectedPosKey = 0 + EasyMoveMng.___
EasyMoveMng.pv = 1*sizeof.rq + EasyMoveMng.expectedPosKey
EasyMoveMng.stableCnt = 4*sizeof.rd + EasyMoveMng.pv
sizeof.EasyMoveMng = 1*sizeof.rd + 3*sizeof.rd + EasyMoveMng.stableCnt

Signals.___ = 0
Signals.stop = 0 + Signals.___
Signals.stopOnPonderhit = 1*sizeof.rb + Signals.stop
sizeof.Signals = 1*sizeof.rb + 14*sizeof.rb + Signals.stopOnPonderhit

Time.___ = 0
Time.startTime = 0 + Time.___
Time.optimumTime = 1*sizeof.rq + Time.startTime
Time.maximumTime = 1*sizeof.rq + Time.optimumTime
sizeof.Time = 1*sizeof.rq + 1*sizeof.rq + Time.maximumTime

ThreadHandle.___ = 0
ThreadHandle.stackAddress = 0 + ThreadHandle.___
ThreadHandle.mutex = 1*sizeof.rq + ThreadHandle.stackAddress
sizeof.ThreadHandle = 1*sizeof.rd + 1*sizeof.rd + ThreadHandle.mutex

Mutex.___ = 0
sizeof.Mutex = 0 + 1*sizeof.rd + 1*sizeof.rd + 1*sizeof.rq + Mutex.___

ConditionalVariable.___ = 0
sizeof.ConditionalVariable = 0 + 1*sizeof.rd + 1*sizeof.rd + 1*sizeof.rq + ConditionalVariable.___

Thread.___ = 0
Thread.mutex = 0 + Thread.___
Thread.sleep1 = 1*sizeof.Mutex + Thread.mutex
Thread.sleep2 = 1*sizeof.ConditionalVariable + Thread.sleep1
Thread.threadHandle = 1*sizeof.ConditionalVariable + Thread.sleep2
Thread.numaNode = 1*sizeof.ThreadHandle + Thread.threadHandle
Thread.bestMoveChanges = 1*sizeof.rq + Thread.numaNode
Thread.PVIdx = 1*sizeof.rq + Thread.bestMoveChanges
Thread.previousScore = 1*sizeof.rd + Thread.PVIdx
Thread.completedDepth = 1*sizeof.rd + Thread.previousScore
Thread.callsCnt = 1*sizeof.rd + Thread.completedDepth
Thread.resetCnt = 1*sizeof.rd + Thread.callsCnt
Thread.searching = 1*sizeof.rd + 1*sizeof.rd + Thread.resetCnt
Thread.exit = 1*sizeof.rb + Thread.searching
Thread.failedLow = 1*sizeof.rb + Thread.exit
Thread.easyMovePlayed = 1*sizeof.rb + Thread.failedLow
Thread.maxPly = 1*sizeof.rb + 1*sizeof.rb + 1*sizeof.rb + Thread.easyMovePlayed
Thread.nodes = 1*sizeof.rb + 1*sizeof.rb + Thread.maxPly
Thread.tbHits = 1*sizeof.rq + Thread.nodes
Thread.idx = 1*sizeof.rq + Thread.tbHits
Thread.rootDepth = 1*sizeof.rd + Thread.idx
Thread.castling_start = 1*sizeof.rd + Thread.rootDepth
Thread.castling_rfrom = 0*sizeof.rb + Thread.castling_start
Thread.castling_rto = 4*sizeof.rb + Thread.castling_rfrom
Thread.castling_path = 4*sizeof.rb + Thread.castling_rto
Thread.castling_ksqpath = 4*sizeof.rq + Thread.castling_path
Thread.castling_knights = 4*8*sizeof.rb + Thread.castling_ksqpath
Thread.castling_kingpawns = 4*sizeof.rq + Thread.castling_knights
Thread.castling_movgen = 4*sizeof.rq + Thread.castling_kingpawns
Thread.castling_rightsMask = 4*sizeof.rd + Thread.castling_movgen
Thread.castling_end = 64*sizeof.rb + Thread.castling_rightsMask
Thread.rootPos = 0*sizeof.rb + Thread.castling_end
sizeof.Thread = 1*sizeof.Pos + Thread.rootPos

NumaNode.___ = 0
NumaNode.nodeNumber = 0 + NumaNode.___
NumaNode.coreCnt = 1*sizeof.rd + NumaNode.nodeNumber
NumaNode.cmhTable = 1*sizeof.rd + NumaNode.coreCnt
NumaNode.parent = 1*sizeof.rq + NumaNode.cmhTable
NumaNode.cpuMask = 1*sizeof.rq + 1*sizeof.rq + NumaNode.parent
sizeof.NumaNode = MAX_LINUXCPUS/64*sizeof.rq + NumaNode.cpuMask

ThreadPool.___ = 0
ThreadPool.threadCnt = 0 + ThreadPool.___
ThreadPool.nodeCnt = 1*sizeof.rd + ThreadPool.threadCnt
ThreadPool.coreCnt = 1*sizeof.rd + ThreadPool.nodeCnt
ThreadPool.threadTable = 1*sizeof.rd + 1*sizeof.rd + ThreadPool.coreCnt
ThreadPool.nodeTable = MAX_THREADS*sizeof.rq + ThreadPool.threadTable
sizeof.ThreadPool = MAX_NUMANODES*sizeof.NumaNode + ThreadPool.nodeTable

IOBuffer.inputBuffer       = 0
IOBuffer.inputBufferSizeB  = 8 + IOBuffer.inputBuffer
IOBuffer.cmdLineStart      = 8 + IOBuffer.inputBufferSizeB
IOBuffer.tmp_i             = 8 + IOBuffer.cmdLineStart
IOBuffer.tmp_j             = 4 + IOBuffer.tmp_i
IOBuffer.tmpBuffer         = 4 + IOBuffer.tmp_j
sizeof.IOBuffer.tmpBuffer  = 512
sizeof.IOBuffer            = 512+IOBuffer.tmpBuffer
