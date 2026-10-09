# Standards Strategy

## 1. Baseline

| Profile | Ngôn ngữ | Mục đích |
|---|---|---|
| `safety-cpp17` | C++17 | Profile học chính, bám MISRA C++:2023 |
| `autosar-cpp14` | C++14 | Compatibility profile, học AUTOSAR C++14/codebase cũ |
| `host-tools` | C++20 nếu cần | Tooling ngoài safety-critical target; không link vào target |

Không dùng feature mới chỉ vì compiler hỗ trợ. Mọi target phải khai báo profile ngay trong README và CMake.

## 2. Thứ tự ưu tiên

1. Requirement và safety constraint của project.
2. MISRA C++:2023 + project Guideline Re-categorization Plan.
3. AUTOSAR C++14 khi target/profile yêu cầu.
4. C++ Core Guidelines cho phần không xung đột.
5. Project style guide (`clang-format`, naming, layout).

Coding style không thay thế coding standard; format đẹp không đồng nghĩa với compliant.

## 3. Project policy ban đầu

- Không undefined behavior; implementation-defined behavior phải được ghi lại.
- Không raw owning pointer; ownership phải thể hiện trong type/interface.
- Không global mutable state.
- Biến phải được khởi tạo; conversion thu hẹp phải explicit và được kiểm tra.
- Public API phải nêu precondition, postcondition, error behavior và thread-safety.
- Resource phải được quản lý bằng RAII.
- Runtime allocation, exception, RTTI và recursion: mặc định cấm ở phần realtime/safety; chỉ mở bằng ADR + evidence.
- Loop/queue/buffer trong đường realtime phải có bound.
- Warning là error; finding không được “tắt cho sạch” nếu chưa phân loại.
- Mọi concurrency primitive phải có ownership và shutdown protocol được tài liệu hóa.

Đây là project policy để học, không phải bản sao đầy đủ của MISRA.

## 4. Compliance artifacts

Mỗi project tạo folder `docs/compliance/` gồm:

```text
compliance/
├── applicability.md
├── guideline_reclassification_plan.md
├── tool_qualification_notes.md
├── analysis_report.md
├── deviation_log.md
└── compliance_summary.md
```

Mỗi deviation phải ghi: ID, guideline ID, location, lý do, phạm vi, risk analysis, mitigation, verification, approver và ngày hết hiệu lực/review lại.

## 5. Tool gates

- Compiler: GCC và Clang, warning nghiêm ngặt.
- Format: `clang-format --dry-run --Werror`.
- Static analysis: clang-tidy/cppcheck để học; analyzer có MISRA coverage khi có license.
- Runtime: ASan + UBSan; TSan cho concurrency; Valgrind khi phù hợp.
- Test: unit, component, integration; coverage chỉ là chỉ báo, không phải bằng chứng đủ.
- CI: không cho qua nếu tool fail hoặc finding chưa được disposition.

Không tuyên bố “MISRA compliant” chỉ dựa vào một lần chạy tool. Compliance còn cần rule coverage, process, deviation handling và evidence.

