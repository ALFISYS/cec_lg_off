<#
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
#>
$f_cpu_list = @(16500000, 16000000, 8000000, 4000000, 2000000, 1000000)
$cec_us_vals = @(400, 800, 1250, 1700, 2050, 3500, 3900, 4300, 4700)
$max_us_cec = $cec_us_vals | Sort-Object -Descending | Select-Object -First 1
$delta0 = 0
$delta0prc = 0
$or1c = 0
$cs0 = 0
$result = @()
foreach ($f_cpu in $f_cpu_list) {
    

    $cs0 = 0
    $cec_jiffle_vals = @()
    foreach($cs0_test in @(1..5)) {
        $div_test = @(0, 1, 8, 64, 256, 1024)[$cs0_test]
        $f_t0 = $f_cpu / $div_test 
        $us_per_tick = 1000000 / $f_t0
        $us_per_256_ticks = $us_per_tick * 256
        if ($us_per_256_ticks -lt $max_us_cec) {
            continue
        }
        $cec_jiffle_vals_test = $cec_us_vals | ForEach-Object { [Math]::Round($_ / $us_per_tick)}
        $cec_deltas_us = @(0..($cec_jiffle_vals_test.Count - 1)) | ForEach-Object { [Math]::Abs($cec_us_vals[$_] - ($us_per_tick * $cec_jiffle_vals_test[$_])) } 
        $cec_deltas_prc = @(0..($cec_deltas_us.Count - 1)) | ForEach-Object { $cec_deltas_us[$_] / $cec_us_vals[$_] * 100  } 
        $delta_test = $cec_deltas_us | Sort-Object -Descending | Select-Object -First 1

        if ($cs0 -eq 0 -or $delta0 -gt $delta_test) {
            $cs0 = $cs0_test
            $delta0 = $delta_test
            $delta0prc = $cec_deltas_prc | Sort-Object -Descending | Select-Object -First 1
            $cec_jiffle_vals = $cec_jiffle_vals_test
        }
    }

    $result_o = @{
        'F_CPU' = $f_cpu
        '  _  ' = '  |  '
        'ΔT0 WORST' = "$([Math]::Round($delta0)) μs ($([Math]::Round($delta0prc, 2))%)"
        '  |  ' = '  |  '
        'T0CS' = $cs0 
        }
    $c = 1
    foreach ($val in $cec_jiffle_vals) {
        $result_o["CEC$($c)"] = $val
        $c++
    }
    
    if ($result_o.CS0 -eq 0) {
        $result_o.CS0 = 'NOT ACCEPTABLE'
    }
    $result += @($result_o)
}

Write-Host -f Yellow ("=" * 60)
Write-Host ""
Write-Host ""
Write-Host ""
$whopts = @{
    ForegroundColor = 'Green'
}

Write-Host @whopts @"
/**
 * CONTENTS OF THIS FILE ARE GENERATED WITH 'gen_constants.h'
*/
"@
Write-Host @whopts "#ifndef F_CPU"
Write-Host @whopts "  #error No F_CPU configured."


for ($i = 0; $i -lt $result.Count; $i++) {

    Write-Host @whopts "#elif F_CPU == $($result[$i]['F_CPU'])"
    Write-Host @whopts "  #define T0CS             ((uint8_t)$($result[$i]['T0CS'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_DB_LH_MIN    ((uint8_t)$($result[$i]['CEC1'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_DB_LH_D1_MAX ((uint8_t)$($result[$i]['CEC2'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_DB_LH_D0_MIN ((uint8_t)$($result[$i]['CEC3'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_DB_LH_MAX    ((uint8_t)$($result[$i]['CEC4'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_DB_HL_MAX    ((uint8_t)$($result[$i]['CEC5'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_SB_LH_MIN    ((uint8_t)$($result[$i]['CEC6'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_SB_LH_MAX    ((uint8_t)$($result[$i]['CEC7'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_SB_HL_MIN    ((uint8_t)$($result[$i]['CEC8'])) // generated using 'helper_constants.ps1'"
    Write-Host @whopts "  #define CEC_SB_HL_MAX    ((uint8_t)$($result[$i]['CEC9'])) // generated using 'helper_constants.ps1'"
}
Write-Host @whopts "#else"
Write-Host @whopts "    #error No preset for your F_CPU (cpu frequency) is pre-calculated. Use 'helper_constants.ps1' and calculate preset for your F_CPU, then add to this place in code."
Write-Host @whopts "#endif"
Write-Host ""
Write-Host ""
Write-Host ""
Write-Host -f Yellow ("=" * 60)