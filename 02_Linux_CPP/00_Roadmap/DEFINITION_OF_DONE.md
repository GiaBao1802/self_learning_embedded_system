# Definition of Done

Một feature chỉ được xem là hoàn thành khi tất cả mục áp dụng đã đạt.

## Requirement

- [ ] Có requirement ID, rationale và acceptance criteria.
- [ ] Functional và non-functional constraints đã rõ.
- [ ] Requirement liên kết tới design, code và test.

## Design

- [ ] Boundary, ownership, lifetime và error flow rõ ràng.
- [ ] Có state/sequence diagram nếu hành vi không tầm thường.
- [ ] Pattern được chọn để giải quyết vấn đề cụ thể; có trade-off.
- [ ] Quyết định quan trọng có ADR.
- [ ] Design review checklist đã hoàn tất.

## Implementation

- [ ] Đúng language/compliance profile.
- [ ] Không warning trên GCC và Clang.
- [ ] Public API có contract và thread-safety note.
- [ ] Không dead code, debug code hoặc suppression vô lý.
- [ ] Complexity, allocation và blocking behavior nằm trong budget.

## Verification

- [ ] Unit/component/integration tests tương ứng risk.
- [ ] Boundary, error path, timeout và shutdown đã được test.
- [ ] Static analysis findings đã fix hoặc disposition.
- [ ] Sanitizer phù hợp đã pass.
- [ ] Traceability matrix được cập nhật.
- [ ] Deviation được review và có mitigation.

## Delivery

- [ ] Build lặp lại được từ folder sạch.
- [ ] README mô tả build/run/test/profile.
- [ ] CI pass và giữ report cần thiết.
- [ ] Release note nêu limitation/known issue.

