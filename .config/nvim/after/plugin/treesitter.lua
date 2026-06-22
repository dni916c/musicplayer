require'nvim-treesitter.configs'.setup {
  ensure_installed = { "c", "lua", "vim", "vimdoc", "javascript", "typescript", "python", "cpp" },
  sync_install = false,
  auto_install = true,

  highlight = {
    enable = true,
  },
}
