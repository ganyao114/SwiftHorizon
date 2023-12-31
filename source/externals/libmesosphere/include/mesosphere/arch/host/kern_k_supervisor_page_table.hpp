#pragma once
#include <mesosphere/kern_common.hpp>
#include <mesosphere/kern_select_cpu.hpp>
#include <mesosphere/arch/host/kern_k_page_table.hpp>

namespace ams::kern::arch::host {

    class KSupervisorPageTable {
        private:
            KPageTable m_page_table;
            u64 m_ttbr0_identity[cpu::NumCores];
        public:
            constexpr KSupervisorPageTable() : m_page_table(util::ConstantInitialize), m_ttbr0_identity() { /* ... */ }

            NOINLINE void Initialize(s32 core_id);

            void Activate() {
                /* Activate, using process id = 0xFFFFFFFF */
                m_page_table.Activate(0xFFFFFFFF);
            }

            void ActivateForInit() {
                this->Activate();

                /* Invalidate entire TLB. */
                cpu::InvalidateEntireTlb();
            }

            Result MapPages(KProcessAddress *out_addr, size_t num_pages, size_t alignment, KPhysicalAddress phys_addr, KProcessAddress region_start, size_t region_num_pages, KMemoryState state, KMemoryPermission perm) {
                R_RETURN(m_page_table.MapPages(out_addr, num_pages, alignment, phys_addr, region_start, region_num_pages, state, perm));
            }

            Result UnmapPages(KProcessAddress address, size_t num_pages, KMemoryState state) {
                R_RETURN(m_page_table.UnmapPages(address, num_pages, state));
            }

            Result MapPageGroup(KProcessAddress *out_addr, const KPageGroup &pg, KProcessAddress region_start, size_t region_num_pages, KMemoryState state, KMemoryPermission perm) {
                R_RETURN(m_page_table.MapPageGroup(out_addr, pg, region_start, region_num_pages, state, perm));
            }

            Result UnmapPageGroup(KProcessAddress address, const KPageGroup &pg, KMemoryState state) {
                R_RETURN(m_page_table.UnmapPageGroup(address, pg, state));
            }

            bool GetPhysicalAddress(KPhysicalAddress *out, KProcessAddress address) const {
                return m_page_table.GetPhysicalAddress(out, address);
            }

            constexpr u64 GetIdentityMapTtbr0(s32 core_id) const { return m_ttbr0_identity[core_id]; }

            void DumpMemoryBlocks() const {
                return m_page_table.DumpMemoryBlocks();
            }

            void DumpPageTable() const {
                return m_page_table.DumpPageTable();
            }

            size_t CountPageTables() const {
                return m_page_table.CountPageTables();
            }
    };

}
