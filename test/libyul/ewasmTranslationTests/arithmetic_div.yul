{
  sstore(0, div(0, 1))
  sstore(1, div(1, not(0)))
  sstore(2, div(0, 0))
  sstore(3, div(1, 2))
  sstore(4, div(0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe, 1))
  sstore(5, div(
    0x8000000000000000000000000000000000000000000000000000000000000000, 1
  ))
  sstore(6, div(not(0), 1))
  sstore(7, div(0xffffffffffffffffffffffffffffffff, 1))
  sstore(8, div(0xffffffffffffffff, 1))
  sstore(9, div(0xffffffffffffffffffffffffffffffff0000000000000000, 1))
  sstore(10, div(0xffffffffffffffffffffffffffffffffffffffffffffffff, 3))
  sstore(11, div(0xffffffffffffffffffffffffffffffff, 3))
  sstore(12, div(0xffffffffffffffff, 3))
  sstore(13, div(0xffffffffffffffffffffffffffffffff0000000000000000, 3))
}
// ----
// Trace:
// Memory dump:
//      0: 000000000000000000000000000000000000000000000000000000000000000d
//     20: 0000000000000000555555555555555555555555555555550000000000000000
// Storage dump:
//   0000000000000000000000000000000000000000000000000000000000000004: fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffe
//   0000000000000000000000000000000000000000000000000000000000000005: 8000000000000000000000000000000000000000000000000000000000000000
//   0000000000000000000000000000000000000000000000000000000000000006: ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
//   0000000000000000000000000000000000000000000000000000000000000007: 00000000000000000000000000000000ffffffffffffffffffffffffffffffff
//   0000000000000000000000000000000000000000000000000000000000000008: 000000000000000000000000000000000000000000000000ffffffffffffffff
//   0000000000000000000000000000000000000000000000000000000000000009: 0000000000000000ffffffffffffffffffffffffffffffff0000000000000000
//   000000000000000000000000000000000000000000000000000000000000000a: 0000000000000000555555555555555555555555555555555555555555555555
//   000000000000000000000000000000000000000000000000000000000000000b: 0000000000000000000000000000000055555555555555555555555555555555
//   000000000000000000000000000000000000000000000000000000000000000c: 0000000000000000000000000000000000000000000000005555555555555555
//   000000000000000000000000000000000000000000000000000000000000000d: 0000000000000000555555555555555555555555555555550000000000000000