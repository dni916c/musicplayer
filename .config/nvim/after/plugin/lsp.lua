require('blink.cmp').setup({
  keymap = { preset = 'default' },        -- <C-y> accept, <C-n>/<C-p> cycle, <C-space> open
  appearance = { nerd_font_variant = 'mono' },
  sources = { default = { 'lsp', 'path', 'snippets', 'buffer' } },
  fuzzy = { implementation = 'lua' },     -- pure-Lua matcher, no Rust binary to fetch (clean on ARM64)
  completion = { documentation = { auto_show = true } },
})

-- Hand blink's completion capabilities to the server so it advertises them
local capabilities = require('blink.cmp').get_lsp_capabilities()

require('lspconfig').clangd.setup({
  capabilities = capabilities,
})

-- IntelliSense-style keymaps, active only where an LSP attaches
vim.api.nvim_create_autocmd('LspAttach', {
  callback = function(e)
    local opts = { buffer = e.buf }
    vim.keymap.set('n', 'gd', vim.lsp.buf.definition, opts)   -- go to definition
    vim.keymap.set('n', 'K',  vim.lsp.buf.hover, opts)        -- hover docs
    vim.keymap.set('n', '<leader>rn', vim.lsp.buf.rename, opts)
    vim.keymap.set('n', '<leader>ca', vim.lsp.buf.code_action, opts)
    vim.keymap.set('n', '[d', function() vim.diagnostic.jump({ count = -1 }) end, opts)
    vim.keymap.set('n', ']d', function() vim.diagnostic.jump({ count =  1 }) end, opts)
  end,
})
